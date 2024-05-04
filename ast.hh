#pragma once
#include <utility>

namespace ast {
template <typename... T> struct Node {
  using Node_t = Node<T...>;
  using Node_p = Node_t *;

  virtual int operator()(size_t k, const T &...) const = 0;

  virtual ~Node() = default;
};
////////////////////////////////////////////////
template <typename... T> struct BinOp : public Node<T...> {
  using typename Node<T...>::Node_p;
  Node_p l_, r_;
  BinOp(Node_p l, Node_p r) : l_(l), r_(r) {}
  BinOp(const BinOp &) = delete;
  BinOp &operator=(const BinOp &) = delete;
  BinOp(BinOp &&rhs) : l_(std::move(rhs.r_)), r_(std::move(rhs.r_)) {
    rhs.r_ = rhs.l_ = nullptr;
  }
  BinOp &operator=(BinOp &&rhs) {
    std::swap(rhs.l_, l_);
  std:
    swap(rhs.r_, r_);
  }
  int operator()(size_t k, const T &...params) const override {
    // return std::max((*l_)(k, params...) - (*r_)(k, params...), 0);
    return std::min((*l_)(k, params...), (*r_)(k, params...));
  }
  ~BinOp() {
    delete l_;
    delete r_;
  }
};

template <typename... T> struct J1 : public Node<T...> {
  using typename Node<T...>::Node_p;
  Node_p c_;
  J1(Node_p c) : c_(c) {}
  J1(J1 &&rhs) : c_(std::move(rhs.c_)) { rhs.c_ = nullptr; }
  J1 &operator=(J1 &&rhs) { std::swap(c_, rhs.c_); }
  J1(const J1 &) = delete; 
  J1 &operator=(const J1 &) = delete;
  int operator()(size_t k, const T &...params) const override {
    return ((*c_)(k, params...) == 1);
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
  int operator()(size_t k, const T &...params) const override {
    return (k + (num_ % k)) % k;
  }
};
} // namespace ast
