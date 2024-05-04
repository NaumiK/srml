#pragma once
#include "ast.hh"
#include <iostream>

template <typename... T> struct P_SDNF {
  void operator()(const ast::Node<T...> &nde, int k) = delete;
};

template <> struct P_SDNF<int> {
  void operator()(const ast::Node<int> &nde, int k) {
    std::cout << "x\tf\n";
    for (int i = 0; i < k; ++i)
      std::cout << i << "\t" << nde(k, i) << "\n";
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
    std::cout << "x\ty\tf\n";
    for (int i = 0; i < k; ++i)
      for (int j = 0; j < k; ++j)
        std::cout << i << "\t" << j << "\t" << nde(k, i, j) << "\n";

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