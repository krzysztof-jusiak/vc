#pragma once

#include "vc/concepts/concept.hpp"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif
#include <dyno.hpp>
using namespace dyno::literals;  // FIXME

#define FNAME(name)                                                                                                     \
  [](auto t, auto r, auto... args) {                                                                                    \
    struct {                                                                                                            \
      using name_t = decltype(__GUNIT_CAT(#name, _s));                                                                  \
      using type_t = boost::hana::basic_type<typename decltype(r)::type(dyno::T&, decltype(args)...)>;                  \
      static auto expr() {                                                                                              \
        return [](auto& self, decltype(args)... args) { return self.name(std::forward<decltype(args)>(args)...); };     \
      }                                                                                                                 \
      static auto constraint() {                                                                                        \
        return [](auto& self, decltype(args)... args) -> decltype(self.name(std::forward<decltype(args)>(args)...)) {}; \
      }                                                                                                                 \
      auto name(decltype(args)... args) {                                                                               \
        return reinterpret_cast<decltype(t)*>(this)->template call<name_t, typename decltype(r)::type>(                 \
            std::forward<decltype(args)>(args)...);                                                                     \
      }                                                                                                                 \
    } _;                                                                                                                \
    return _;                                                                                                           \
  }

#define $fname FNAME

template <class R, class... Ts, class X>
auto CallableImpl(aux::type<R(Ts...)>, X x) {
  return requires_expr(std::true_type{}, [&x] { return x; }, [](auto&&, aux::identity<R>, Ts...) {}, [](...) {});
}

template <class T, class X>
auto Callable(X x) {
  return CallableImpl(aux::type<T>{}, x);
}
