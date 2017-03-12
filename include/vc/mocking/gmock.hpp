#pragma once

#include "GUnit/GMock.h"

using namespace testing;

struct MockImplBase {
  MockImplBase() = default;

  template<class TName, class R, class... TArgs>
  auto call(TArgs&&... args) {
    const auto it = fs.find(TName::c_str());
    if (it == fs.end()) {
      fs[TName::c_str()] = std::make_unique<FunctionMocker<R (TArgs...)>>();
    }
    auto* f = static_cast<FunctionMocker<R(TArgs...)> *>(fs[TName::c_str()].get());
    f->SetOwnerAndName(this, TName::c_str());
    return f->Invoke(std::forward<TArgs>(args)...);
  }

  template <class TName, class R, class... TArgs>
  decltype(auto) gmock_call_impl(const detail::identity_t<Matcher<TArgs>> &... args) {
    const auto it = fs.find(TName::c_str());
    auto *ptr = [&it, this] {
      if (it == fs.end()) {
        fs[TName::c_str()] = std::make_unique<FunctionMocker<R(TArgs...)>>();
        return static_cast<FunctionMocker<R(TArgs...)> *>(fs[TName::c_str()].get());
      }
      return static_cast<FunctionMocker<R(TArgs...)> *>(it->second.get());
    }();

    ptr->RegisterOwner(this);
    return ptr->With(args...);
  }

  template <class TName, class R, class B, class... TArgs>
  decltype(auto) gmock_call(R (B::*)(TArgs...), const detail::identity_t<Matcher<TArgs>> &... args) {
    return gmock_call_impl<TName, R, TArgs...>(args...);
  }

  template <class TName, class R, class B, class... TArgs>
  decltype(auto) gmock_call(R (B::*)(TArgs...) const, const typename detail::identity_t<Matcher<TArgs>> &... args) {
    return gmock_call_impl<TName, R, TArgs...>(args...);
  }

  std::unordered_map<std::string, std::unique_ptr<internal::UntypedFunctionMockerBase>> fs;
};

template <class>
struct MockImpl;

template <class T>
struct Mock : MockImpl<typename T::type> {
  using MockImpl<typename T::type>::MockImpl;
};

template <class... Ts>
struct MockImpl<aux::type_list<Ts...>>
  : MockImplBase, base<typename Ts::callable, typename Ts::call, aux::function_traits_t<decltype(&Ts::parameters::template operator()<_1>), MockImplBase>>::type...
{
  using type = MockImpl;
};

