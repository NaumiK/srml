#include <exception>
#include <iostream>

#include "ast.hh"
#include "psdnf.hh"
#include "tokens.hh"
#include <FlexLexer.h>
#include <string>
#include <vector>
#include "n_term.hh"
#include "save_set.hh"

// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() { return 0; }

template <typename... Tp> void plot() {
  FlexLexer *lexer = new yyFlexLexer;
  ast::Node<Tp...> *tree = nullptr;
  size_t k;
  std::cout << "k: ";
  std::cin >> k;
  std::cout << "Function: ";
  auto tmp = static_cast<Flex::Tokens>(lexer->yylex());
  Flex::n_term::E(tree, lexer, tmp);
  delete lexer;
  P_SDNF<Tp...>{}(*tree, k);
  std::cout << "\n";
  std::cin.clear();
  std::cin.ignore();
  std::cout << (char)std::cin.peek();
  std::cout << ((Save_set<Tp...>{}(*tree, k)) ? "Set saved" : "Set not saved")
            << "\n";
}

int main() try {
  std::cout << "M: ";
  size_t m;
  std::cin >> m;
  if (m % 2 == 0)
    plot<int, int>();
  else
    plot<int>();
} catch (std::exception &err) {
  std::cerr << err.what() << std::endl;
}