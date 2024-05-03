#include <exception>
#include <iostream>

#include "ast.hh"
#include "tokens.hh"
#include <FlexLexer.h>
#include <string>

// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() { return 0; }

namespace Flex {
namespace n_term {
void Et(ast::Node<int> *&nde, FlexLexer *lexer, Flex::Tokens &tmp);
void T(ast::Node<int> *&nde, FlexLexer *lexer, Flex::Tokens &tmp);
void E(ast::Node<int> *&nde, FlexLexer *lexer, Flex::Tokens &tmp) {
  switch (tmp) {
  case UNKNOWN:
    throw std::exception("Unknown lexema");
    break;
  case CONST:
  case VALUE:
  case UNROP:
  case LBRAC:
    T(nde, lexer, tmp);
    Et(nde, lexer, tmp);
    break;
  default:
    throw std::exception("Wrong lexema");
  }
}
void Et(ast::Node<int> *&nde, FlexLexer *lexer, Flex::Tokens &tmp) {
  switch (tmp) {
  case UNKNOWN:
    throw std::exception("Unknown lexema");
    break;
  case RBRAC:
  case ENDCH:
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    break;
  case BINOP: {
    auto op = new ast::BinOp<int>{nde, nullptr};
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    nde = op;
    T(op->r_, lexer, tmp);
    Et(nde, lexer, tmp);
  } break;
  default:
    throw std::exception("Wrong lexema");
  }
}
void T(ast::Node<int> *&nde, FlexLexer *lexer, Flex::Tokens &tmp) {
  switch (tmp) {
  case UNKNOWN:
    throw std::exception("Unknown lexema");
    break;
  case CONST:
    nde = new ast::Const<int>{std::stoi(lexer->YYText())};
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    break;
  case VALUE:
    nde = new ast::Var<0, int>{};
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    break;
  case UNROP: {
    auto op = new ast::J1<int>{nullptr};
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    nde = op;
    E(op->c_, lexer, tmp);
  } break;
  case LBRAC: {
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    E(nde, lexer, tmp);
  } break;
  default:
    throw std::exception("Wrong lexema");
  }
}
} // namespace n_term
} // namespace Flex

template <typename... T> struct P_SDNF {
  void operator()(const ast::Node<T...> &nde, int k) = delete;
};

template <> struct P_SDNF<int> {
  void operator()(const ast::Node<int> &nde, int k) {
    for (int i = 0; i < k; ++i) {
      auto res = nde(k, i);
      if (res != 0)
        std::cout << res << " & "
                  << "J_" << i << "(x)"
                  << " v ";
    }
  }
};

template <> struct P_SDNF<int, int> {
  void operator()(const ast::Node<int, int> &nde, int k) {
    for (int i = 0; i < k; ++i)
      for (int j = 0; j < k; ++j) {
        auto res = nde(k, i, j);
        if (res != 0)
          std::cout << res << " & "
                    << "J_" << i << "(x)"
                    << " & "
                    << "J_" << j << "(y)"
                    << " v ";
      }
  }
};

int main() try {
  FlexLexer *lexer = new yyFlexLexer;
  ast::Node<int> *tree = nullptr;
  auto tmp = static_cast<Flex::Tokens>(lexer->yylex());
  Flex::n_term::E(tree, lexer, tmp);
  P_SDNF<int>{}(*tree, 5);
  std::cout << "\n";
  P_SDNF<int>{}(ast::BinOp<int>{new ast::Var<0, int>{}, new ast::Const<int>{2}}, 5);
  delete lexer;
} catch (std::exception &err) {
  std::cerr << err.what() << std::endl;
}