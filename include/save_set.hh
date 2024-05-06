#pragma once
#include "ast.hh"

template <typename... Tp> bool save_set(const ast::Node<Tp...> &nde, size_t k);
template <> bool save_set<int>(const ast::Node<int> &nde, size_t k);
template <> bool save_set<int, int>(const ast::Node<int, int> &nde, size_t k);
