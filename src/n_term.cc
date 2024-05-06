#include "ast.hh"
#include <n_term.hh>

template <>
ast::Node<int> *Flex::n_term::detail::choose_var<int>(std::string_view inp) {
  return new ast::Var<0, int>{};
}

template<>
ast::Node<int, int> *Flex::n_term::detail::choose_var<int, int>(std::string_view inp) {
    if (inp[0] == 'x')
      return new ast::Var<0, int, int>;
    else
      return new ast::Var<1, int, int>;
}