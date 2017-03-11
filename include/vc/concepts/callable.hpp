#pragma once

#include "vc/concepts/concept.hpp"

template<class R, class... Ts, class X>
auto CallableImpl(type<R(Ts...)>, X x) {
  return REQUIRES3(x, auto&&, identity<R>, Ts...)(int{}); // TODO is empty __VA_ARGS__
}

template<class T, class X>
auto Callable(X x) {
  return CallableImpl(type<T>{}, x);
};

