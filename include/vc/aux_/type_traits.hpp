#pragma once

#include <type_traits>

namespace aux {

template <class...>
struct type {};
template <class T>
struct identity {
  using type = T;
};
template <class...>
struct type_list {};

template <class, class>
struct function_traits;

template <class R, class B, class T1, class... Ts, class T>
struct function_traits<R (B::*)(T1, Ts...) const, T> {
  using type = type_list<T, Ts...>;
};

template <class T, class U>
using function_traits_t = typename function_traits<T, U>::type;

template <class...>
using void_t = void;

template <class>
using always = std::true_type;
}

template <class... Ts>
aux::type<Ts...> type{};
