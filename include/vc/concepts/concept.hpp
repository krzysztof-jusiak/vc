#pragma once

#include <dyno.hpp>
#include "vc/aux_/type_traits.hpp"
#include "vc/aux_/utility.hpp"

using namespace dyno::literals; // FIXME

#define REQUIRES__(...) typename std::enable_if<(__VA_ARGS__), bool>::type = true
#define REQUIRES_IMPL(...) is_valid_expr(__VA_ARGS__)) {}) _Pragma("GCC diagnostic pop")
#define REQUIRES(...)\
  _Pragma("GCC diagnostic push")\
  _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")\
  _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")\
  _Pragma("GCC diagnostic ignored \"-Wunused-local-typedef\"")\
expr(std::false_type{}, []{}, [](__VA_ARGS__){}, [](__VA_ARGS__) -> decltype( REQUIRES_IMPL

__attribute__((unused)) auto is_valid_expr = [](auto &&...) {};

struct _1 {
  template<class, class R, class... TArgs> R call(TArgs&&...);
};

template<class, class =void>
struct models : std::false_type {};

template<class TConcept, class... Ts>
struct models<TConcept(Ts...), void_t<
    decltype(TConcept{}(std::declval<Ts>()...))
>> : std::true_type
{};

template<class, class T, class>
struct base {
  using type = type<T>;
};

template<class T, class... Ts>
struct base<std::true_type, T, type_list<Ts...>> {
  using type = decltype(expr_wrapper<T>{}()(std::declval<Ts>()...));
};

template<class, class, class>
struct constraint;

template<class T, class T1, class T2, class... Ts>
struct constraint<std::true_type, T, type_list<T1, T2, Ts...>> {
  using expr = typename base<std::true_type, typename T::call, function_traits_t<decltype(&T::parameters::template operator()<_1>), _1>>::type;
  using constr = decltype(expr::constraint());
  using type = typename models<expr_wrapper<constr>(T1, Ts...)>::type;
};

template<class T, class... Ts>
struct constraint<std::false_type, T, type_list<Ts...>>
  : models<expr_wrapper<typename T::constraint>(Ts...)>::type
{ };

template<class T, class... Ts>
constexpr auto satisfies_impl(type_list<Ts...>) {
  return std::is_same<
    type_list<always<Ts>...>,
    type_list<typename constraint<typename Ts::callable, Ts, function_traits_t<decltype(&Ts::parameters::template operator()<_1>), T>>::type...>
  >::value;
}

template<class T, class TConcept>
constexpr auto satisfies(const TConcept&) {
  return satisfies_impl<T>(typename TConcept::type{});
}

template<class... Ts>
struct and_ {
  using type = type_list<Ts...>;
  template<class U> auto operator&&(U) {
    return and_<Ts..., U>{};
  }
};

template<class T, class T1, class T2, class T3>
struct expr_impl {
  template<class U> auto operator&&(U) {
      return and_<expr_impl, U>{};
  }

  using callable = T;
  using call = T1;
  using parameters = T2;
  using constraint = T3;
  using type = type_list<expr_impl>;
};

template<class T, class T1, class T2, class T3>
auto expr(T, T1, T2, T3) {
  return expr_impl<T, T1, T2, T3>{};
};
