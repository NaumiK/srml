#include <functional>
#include <iostream>
#include <memory>
#include <regex>
#include <vector>


namespace ast {
template <typename... T> struct Node {
  using Node_t = Node<T...>;
  using Node_p = std::unique_ptr<Node_t>;

  virtual int operator()(size_t k, const T &...) const = 0;

  virtual ~Node() = default;
};
////////////////////////////////////////////////
template <typename... T> struct BinOp : public Node<T...> {
  using typename Node<T...>::Node_p;
  Node_p l_, r_;
  BinOp(Node_p &&l, Node_p &&r) : l_(std::move(l)), r_(std::move(r)) {}
  int operator()(size_t k, const T &...params) const override {
    return std::max((*l_)(k, params...) - (*r_)(k, params...), 0);
  }
};

template <typename... T> struct J1 : public Node<T...> {
  using typename Node<T...>::Node_p;
  Node_p c_;
  J1(Node_p &&c) : c_(std::move(c)) {}
  int operator()(size_t k, const T &...params) const override {
    return ((*c_)(k, params...) == 1) * (k - 1);
  }
};
////////////////////////////////////////////////
namespace detail {
template <size_t which, typename T0, typename... T>
struct VarBck : public Node<T...> {
  int operator()(size_t k, const T &...params) const override {
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
  int operator()(size_t k, const T &...params) const override {
    return detail::VarBck<which, T...>{}(k, params...);
  }
};

template <typename... T> struct Const : public Node<T...> {
private:
  int num_;

public:
  Const(int num) : num_(num) {}
  int operator()(size_t k, const T &...params) {
    return (num_ + (num_ % k)) % k;
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

template <typename... T> struct P_SDNF {
  void operator()(const ast::Node<T...> &nde, int k) = delete;
};

template <> struct P_SDNF<int> {
  void operator()(const ast::Node<int> &nde, int k) {
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

#if 1
int main() {
  auto &&bo =
      ast::BinOp<int, int>{std::make_unique<ast::J1<int, int>>(
                               std::make_unique<ast::Var<0, int, int>>()),
                           std::make_unique<ast::Var<0, int, int>>()};
  P_SDNF<int, int>{}(bo, 3);
  return 0;
}
#endif