#pragma once

#include "vc/concepts/concept.hpp"
#include <dyno.hpp>
using namespace dyno::literals; // FIXME

#define FNAME(name)\
  [](auto t, auto r, auto... args) {\
    struct {\
      using name_t = decltype(__GUNIT_CAT(#name, _s)); \
      using type_t = boost::hana::basic_type<typename decltype(r)::type (const dyno::T &, decltype(args)...)>;\
      static auto expr() {\
        return [](const auto& self, decltype(args)... args) { return self.name(std::forward<decltype(args)>(args)...); };\
      }\
      static auto constraint() {\
        return [](const auto& self, decltype(args)... args) -> decltype(self.name(std::forward<decltype(args)>(args)...)) {};\
      }\
      auto name(decltype(args)... args) {\
        return reinterpret_cast<decltype(t)*>(this)->template\
          call<name_t, typename decltype(r)::type>(std::forward<decltype(args)>(args)...);\
      }\
    } _;\
    return _;\
  }

#define REQUIRES3(p, ...) \
  _Pragma("GCC diagnostic push")\
  _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")\
  _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")\
  _Pragma("GCC diagnostic ignored \"-Wunused-local-typedef\"")\
      expr(\
          std::true_type{}\
        , [&] { return p; }\
        , [](__VA_ARGS__){}\
        , [](__VA_ARGS__) -> decltype( REQUIRES_IMPL

template<class R, class... Ts, class X>
auto CallableImpl(type<R(Ts...)>, X x) {
  return REQUIRES3(x, auto&&, identity<R>, Ts...)(int{}); // TODO is empty __VA_ARGS__
}

template<class T, class X>
auto Callable(X x) {
  return CallableImpl(type<T>{}, x);
};

