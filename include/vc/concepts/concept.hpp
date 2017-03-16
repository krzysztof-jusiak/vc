#pragma once

#include "vc/aux_/type_traits.hpp"
#include "vc/aux_/utility.hpp"

#define __REQUIRES_IMPL(...) is_valid_expr(__VA_ARGS__)) {})

#define REQUIRES(...) typename std::enable_if<(__VA_ARGS__), bool>::type = true
#define REQUIRES_EXPR(...) requires_expr(std::false_type{}, []{}, [](__VA_ARGS__){}, [](__VA_ARGS__) -> decltype( __REQUIRES_IMPL

#define $requires REQUIRES_EXPR

__attribute__((unused)) auto is_valid_expr = [](auto &&...) {};

struct _1 {
  template <class, class R, class... TArgs>
  R call(TArgs &&...);
};

template <class, class = void>
struct models : std::false_type {};

template <class TConcept, class... Ts>
struct models<TConcept(Ts...), aux::void_t<decltype(std::declval<TConcept>()(std::declval<Ts>()...))>> : std::true_type {};

template <class, class T, class>
struct base {
  using type = aux::type<T>;
};

template <class T, class... Ts>
struct base<std::true_type, T, aux::type_list<Ts...>> {
  using type = decltype(std::declval<T>()()(std::declval<Ts>()...));
};

template <class, class, class>
struct constraint;

template <class T, class T1, class T2, class... Ts>
struct constraint<std::true_type, T, aux::type_list<T1, T2, Ts...>> {
  using expr = typename base<std::true_type, typename T::call,
                             aux::function_traits_t<decltype(&T::parameters::template operator()<_1>), _1>>::type;
  using constr = decltype(expr::constraint());
  using type = typename models<constr(T1, Ts...)>::type;
};

template <class T, class... Ts>
struct constraint<std::false_type, T, aux::type_list<Ts...>> : models<typename T::constraint(Ts...)>::type {};

template <class T, class... Ts>
constexpr auto satisfies_impl(aux::type_list<Ts...>) {
  return std::is_same<aux::type_list<aux::always<Ts>...>,
                      aux::type_list<typename constraint<
                          typename Ts::callable, Ts,
                          aux::function_traits_t<decltype(&Ts::parameters::template operator()<_1>), T>>::type...>>::value;
}

template <class... Ts>
struct and_ {
  using type = aux::type_list<Ts...>;

  template <class U>
  auto operator&&(U) {
    return and_<Ts..., U>{};
  }

  template <class T>
  constexpr bool operator()(aux::type<T>) const {
    return satisfies_impl<T>(type{});
  }

  constexpr auto operator()() const { return *this; }
};

template <class T, class T1, class T2, class T3>
struct requires_expr_impl {
  template <class U>
  auto operator&&(U) {
    return and_<requires_expr_impl, U>{};
  }

  using callable = T;
  using call = T1;
  using parameters = T2;
  using constraint = T3;
  using type = aux::type_list<requires_expr_impl>;

  template <class U>
  constexpr bool operator()(aux::type<U>) const {
    return satisfies_impl<U>(type{});
  }

  constexpr auto operator()() const { return *this; }
};

template <class T, class T1, class T2, class T3>
auto requires_expr(T, T1, T2, T3) {
  return requires_expr_impl<T, T1, T2, T3>{};
}
