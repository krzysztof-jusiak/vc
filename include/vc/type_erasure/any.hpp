#pragma once

#include <dyno.hpp>
#include "vc/aux_/type_traits.hpp"
#include "vc/aux_/utility.hpp"

template<class TCallable, class T>
struct accept {
  using type = aux::type_list<
    typename base<TCallable, typename T::call, aux::function_traits_t<decltype(&T::parameters::template operator()<_1>), _1>>::type
  >;
};

template<class T>
struct accept<std::false_type, T> {
  using type = aux::type_list<>;
};

template<class... Ts>
using concept = dyno::concept<boost::hana::pair<typename Ts::name_t, typename Ts::type_t>...>;

template<class... Ts>
struct any_impl_base {
  using concepts_t = aux::join_t<typename accept<typename Ts::callable, Ts>::type...>;
  using concept_t = aux::apply_t<concept, concepts_t>;

  template<class T, class... Tx>
  any_impl_base(T t, aux::type_list<Tx...>)
    : poly_{t, dyno::make_concept_map(typename Tx::name_t{} = Tx::expr()...)}
  {}

  template<class T>
  any_impl_base(T t) // non explicit
    : any_impl_base(t, concepts_t{})
  { }

  template<class TName, class R, class... TArgs>
  auto call(TArgs&&... args) {
    return poly_.virtual_(TName{})(poly_, std::forward<TArgs>(args)...);
  }

  dyno::poly<concept_t> poly_;
};

template <class>
struct any_impl;

template <class TConcept>
struct any : any_impl<typename TConcept::type> {
  template<class T, REQUIRES__(TConcept{}(type<T>))>
  any(T t)
   : any_impl<typename TConcept::type>{t}
  { }
};

template <class... Ts>
struct any_impl<aux::type_list<Ts...>>
  : any_impl_base<Ts...>, base<typename Ts::callable, typename Ts::call, aux::function_traits_t<decltype(&Ts::parameters::template operator()<_1>), any_impl_base<Ts...>>>::type...
{
  using any_impl_base<Ts...>::any_impl_base;
};
