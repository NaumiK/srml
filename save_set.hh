#pragma once
#include <vector>
#include "ast.hh"
#include <iostream>


template <typename... T> struct Save_set {
  bool operator()(const ast::Node<T...> &nde, size_t k) = delete;
};

template <> struct Save_set<int> {
  bool operator()(const ast::Node<int> &nde, size_t k) {
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
};

template <> struct Save_set<int, int> {
  bool operator()(const ast::Node<int, int> &nde, size_t k) {
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
          std::cout << i << " " << j << " " << nde(k, i, j) << " " << v[nde(k, i, j)] <<"\n";
        if (v[i] && v[j] && !v[nde(k, i, j)])
          return false;
      }
    return true;
  }
};

