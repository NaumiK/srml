#include "save_set.hh"
#include <iostream>
#include <vector>

template <> bool save_set<int>(const ast::Node<int> &nde, size_t k) {
  std::vector<bool> v(k, false);
  std::cout << "N: ";
  size_t n;
  std::cin >> n;
  for (size_t i = 0; i < n; ++i) {
    size_t tmp;
    std::cin >> tmp;
    v[tmp % k] = true;
  }
  for (size_t i = 0; i < k; ++i)
    if (v[i] && !v[nde(k, i)])
      return false;
  return true;
}

template <> bool save_set<int, int>(const ast::Node<int, int> &nde, size_t k) {
  std::vector<bool> v(k, false);
  std::cout << "N: ";
  size_t n;
  std::cin >> n;
  for (size_t i = 0; i < n; ++i) {
    size_t tmp;
    std::cin >> tmp;
    v[tmp % k] = true;
  }
  for (size_t i = 0; i < k; ++i)
    for (size_t j = 0; j < k; ++j) {
      if (v[i] && v[j] && !v[nde(k, i, j)])
        return false;
    }
  return true;
}