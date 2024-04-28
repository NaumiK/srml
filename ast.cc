#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <regex>

namespace ast {
template <typename... T> struct Node {
  using Node_t = Node<T...>;
  using Node_p = std::unique_ptr<Node_t>;

  virtual int operator()(size_t k, const T &...) = 0;

  virtual ~Node() = default;
};
////////////////////////////////////////////////
template <typename... T> struct BinOp : public Node<T...> {
  using typename Node<T...>::Node_p;
  Node_p l_, r_;
  BinOp(Node_p &&l, Node_p &&r) : l_(std::move(l)), r_(std::move(r)) {}
  int operator()(size_t k, const T &...params) override {
    return std::max((*l_)(k, params...) - (*r_)(k, params...), 0);
  }
};

template <typename... T> struct J1 : public Node<T...> {
  using typename Node<T...>::Node_p;
  Node_p c_;
  J1(Node_p &&c) : c_(std::move(c)) {}
  int operator()(size_t k, const T &...params) override {
    return ((*c_)(k, params...) == 1) * (k - 1);
  }
};
////////////////////////////////////////////////
namespace detail {
template <size_t which, typename T0, typename... T>
struct VarBck : public Node<T...> {
  int operator()(size_t k, const T &...params) override {
    return operator()(k, params...);
  }
  int operator()(size_t k, const T0 &p1, const T &...params) {
    return VarBck<which - 1, T...>{}(k, params...);
  }
};

template <typename T0, typename... T> struct VarBck<0, T0, T...> {
  int operator()(size_t k, const T0 &p1, const T &...params) { return p1 % k; }
};
} // namespace detail

template <size_t which, typename... T> struct Var : public Node<T...> {
  int operator()(size_t k, const T &...params) {
    return detail::VarBck<which, T...>{}(k, params...);
  }
};

Node<int, int> *test(Node<int, int> *nde) {
  std::vector<std::function<std::unique_ptr<Node<int, int>>()>> nc_ = {
      []() { return std::make_unique<BinOp<int, int>>(nullptr, nullptr); },
      []() { return std::make_unique<J1<int, int>>(nullptr); },
      []() { return std::make_unique<Var<0, int, int>>(); },
      []() { return std::make_unique<Var<1, int, int>>(); }};
  int k;
  std::cin >> k;
  auto tmp = nc_[k]();

  return nullptr;
}

template <typename... T> struct AST final {
  std::unique_ptr<Node<T...>> r_;
  AST(const AST &rhs) = delete;
  AST &operator=(const AST &rhs) = delete;
  AST(AST &&rhs) = default;
  AST &operator=(AST &&rhs) = default;
  ~AST() = default;
};

} // namespace ast

#if 0
int main() {
  auto &&bo =
      ast::BinOp<int, int>{std::make_unique<ast::J1<int, int>>(
                               std::make_unique<ast::Var<0, int, int>>()),
                           std::make_unique<ast::Var<0, int, int>>()};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      std::cout << i << " " << j << " " << (bo)(3, i, j) << "\n";

  return 0;
}
#endif