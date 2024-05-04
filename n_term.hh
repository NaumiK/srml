#pragma once
#include <FlexLexer.h>
#include "ast.hh"
#include "tokens.hh"
#include <string>
namespace Flex {
namespace n_term {
template <typename... Tp>
void Et(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp);
template <typename... Tp> struct T;
template <typename... Tp>
void E(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp) {
  switch (tmp) {
  case UNKNOWN:
    throw std::exception("Unknown lexema");
    break;
  case CONST:
  case VALUE:
  case UNROP:
  case LBRAC:
    T<Tp...>{}(nde, lexer, tmp);
    Et(nde, lexer, tmp);
    break;
  default:
    throw std::exception("Wrong lexema");
  }
}
template <typename... Tp>
void Et(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp) {
  switch (tmp) {
  case UNKNOWN:
    throw std::exception("Unknown lexema");
    break;
  case RBRAC:
  case ENDCH:
    // tmp = static_cast<Flex::Tokens>(lexer->yylex());
    break;
  case BINOP: {
    auto op = new ast::BinOp<Tp...>{nde, nullptr};
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    nde = op;
    T<Tp...>{}(op->r_, lexer, tmp);
    Et(nde, lexer, tmp);
  } break;
  default:
    throw std::exception("Wrong lexema");
  }
}
template <typename... Tp> struct T {
  void operator()(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp);
};
template <> struct T<int> {
  void operator()(ast::Node<int> *&nde, FlexLexer *lexer, Flex::Tokens &tmp) {
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
};
template <> struct T<int, int> {
  void operator()(ast::Node<int, int> *&nde, FlexLexer *lexer,
                  Flex::Tokens &tmp) {
    switch (tmp) {
    case UNKNOWN:
      throw std::exception("Unknown lexema");
      break;
    case CONST:
      nde = new ast::Const<int, int>{std::stoi(lexer->YYText())};
      tmp = static_cast<Flex::Tokens>(lexer->yylex());
      break;
    case VALUE:
      if (lexer->YYText()[0] == 'x')
        nde = new ast::Var<0, int, int>{};
      else
        nde = new ast::Var<1, int, int>{};
      tmp = static_cast<Flex::Tokens>(lexer->yylex());
      break;
    case UNROP: {
      auto op = new ast::J1<int, int>{nullptr};
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
};
} // namespace n_term
} // namespace Flex
