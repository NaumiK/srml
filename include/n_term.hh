#pragma once
#include "ast.hh"
#include "tokens.hh"
#include <FlexLexer.h>
#include <array>
#include <charconv>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace Flex {
namespace n_term {
namespace detail {
template <typename... Tp> struct GenL {
  struct Gen {
    virtual ast::Node<Tp...> *gen() = 0;
    virtual ~Gen() = default;
  };
  inline static std::array<std::unique_ptr<GenL<Tp...>::Gen>, sizeof...(Tp)> arr;
  inline static bool init = false;
  template <size_t N> struct GenN : GenL<Tp...>::Gen {
    ast::Node<Tp...> *gen() { return new ast::Var<N, Tp...>; }
  };
  template <size_t... Ns> void generate_arr(std::index_sequence<Ns...>) {
    std::array<std::unique_ptr<GenL<Tp...>::Gen>, sizeof...(Tp)> tmp{
        std::make_unique<GenN<Ns>>()...};
    std::swap(tmp, arr);
  }
  GenL() {
    if (init)
      return;
    generate_arr(std::make_index_sequence<sizeof...(Tp)>());
    init = true;
  }
};
// Общий случай: x_1, x_2, ..., x_n
template <typename... Tp> ast::Node<Tp...> *choose_var(std::string_view inp) {
  GenL<Tp...>{};
  if (inp.starts_with("x_"))
    inp.remove_prefix(2);
  int val = 0;
  auto [ptr, ec] = std::from_chars(inp.data(), inp.data() + inp.length(), val);

  return ec == std::errc() ? GenL<Tp...>::arr[val % sizeof...(Tp)]->gen() : nullptr;
}
template <> ast::Node<int> *choose_var<int>(std::string_view inp);
template <> ast::Node<int, int> *choose_var<int, int>(std::string_view inp);
} // namespace detail

/*
Определил следующие переходы:
E -> T Et
Et -> & T Et | ∅
T -> (E) | j_1(E) | L --- число или переменная
Вывел следующую таблицу переходов, по ней построил функции:
    id    &       j_1(      (       )       $
E   T Et  -       T Et      T Et    -       -
Et  -     & T Et  -         -       ∅       ∅
T   id    -       j_1(E     (E      -       -
$ --- окончание строки
Пример:
E -> T Et -> (E) Et -> (T E_t) Et -> (x & T Et) Et -> 
  (x & 2 Et) Et -> (x & 2) & T Et -> (x & 2) & j_1(E) Et ->
  (x & 2) & j_1(T Et) Et -> (x & 2) & j_1(y Et) Et -> (x & 2) & j_1(y) Et -> (x & 2) & j_1(y)
*/
template <typename... Tp>
void E(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp);
template <typename... Tp>
void Et(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp);
template <typename... Tp>
void T(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp);

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
    T<Tp...>(nde, lexer, tmp);
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
    throw std::logic_error("Unknown lexema");
    break;
  case RBRAC:
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    break;
  case ENDCH:
    break;
  case BINOP: {
    auto op = new ast::BinOp<Tp...>{nde, nullptr};
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    nde = op;
    T<Tp...>(op->r_, lexer, tmp);
    Et(nde, lexer, tmp);
  } break;
  default:
    throw std::invalid_argument("Wrong lexema");
  }
}

template <typename... Tp>
void T(ast::Node<Tp...> *&nde, FlexLexer *lexer, Flex::Tokens &tmp) {
  switch (tmp) {
  case UNKNOWN:
    throw std::exception("Unknown lexema");
    break;
  case CONST:
    nde = new ast::Const<Tp...>{std::stoi(lexer->YYText())};
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
    break;
  case VALUE: {
    auto nde_tmp = detail::choose_var<Tp...>(lexer->YYText());
    std::swap(nde, nde_tmp);
    tmp = static_cast<Flex::Tokens>(lexer->yylex());
  } break;
  case UNROP: {
    auto op = new ast::J1<Tp...>{nullptr};
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
