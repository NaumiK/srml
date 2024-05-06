#include "psdnf.hh"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <vector>

namespace {
std::ostream_iterator<std::string> v_print(std::cout, " v ");
}

template <> void print_sdnf<int>(const ast::Node<int> &nde, int k) {
  auto pel = [](const std::pair<int, int> &val) {
    std::stringstream ss;
    ss << val.second << " & "
       << "J_" << val.first << "(x)";
    return ss.str();
  };

  std::vector<std::pair<int, int>> v_tmp;
  std::cout << "x\tf\n";
  for (int x = 0; x < k; ++x) {
    auto res = nde(k, x);
    std::cout << x << '\t' << res << '\n';
    if (res != 0)
      v_tmp.emplace_back(x, res);
  }
  if (!v_tmp.empty()) {
    std::transform(v_tmp.begin(), std::prev(v_tmp.end()), v_print, pel);
    std::cout << pel(v_tmp.back());
  }
}

template <> void print_sdnf<int, int>(const ast::Node<int, int> &nde, int k) {
  auto pel = [](const std::tuple<int, int, int> &val) {
    auto [x, y, res] = val;
    std::stringstream ss;
    ss << res << " & "
       << "J_" << x << "(x)"
       << " & "
       << "J_" << y << "(y)";
    return ss.str();
  };

  std::vector<std::tuple<int, int, int>> v_tmp;
  std::cout << "x\ty\tf\n";
  for (int x = 0; x < k; ++x)
    for (int y = 0; y < k; ++y) {
      auto res = nde(k, x, y);
      std::cout << x << '\t' << y << '\t' << res << '\n';
      if (res != 0)
        v_tmp.emplace_back(x, y, res);
    }
  if (!v_tmp.empty()) {
    std::transform(v_tmp.begin(), std::prev(v_tmp.end()), v_print, pel);
    std::cout << pel(v_tmp.back());
  }
}