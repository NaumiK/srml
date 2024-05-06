#pragma once
#include "ast.hh"

template <typename... T> void print_sdnf(const ast::Node<T...> &nde, int k);

template <> void print_sdnf<int>(const ast::Node<int> &, int k);
template <> void print_sdnf<int, int>(const ast::Node<int, int> &, int k);
