#pragma once
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <tuple>

template <typename Return, typename... Args>
using reference_to_global_function_t = Return(&)(Args...);

template <typename Return, typename Class, typename... Args>
using pointer_to_member_function_t = Return(Class::*)(Args...);

template <typename F> struct ArgumentInfo;

template  <typename... Args>
struct ArgumentInfoBase
{
  constexpr static auto argument_list_length = std::tuple_size<std::tuple<Args...>>::value;
  constexpr static auto last_argument_index = argument_list_length - 1;

  template <size_t N>
  using nth_argument_t = typename std::tuple_element<N, std::tuple<Args...>>::type;

  using first_argument_t = typename nth_argument_t<0>;
  using last_argument_t = typename nth_argument_t<last_argument_index>;
};

template <typename Return, typename... Args>
struct ArgumentInfo<reference_to_global_function_t<Return, Args...>> : public ArgumentInfoBase<Args...> {};

template <typename Return, typename Class, typename... Args>
struct ArgumentInfo<pointer_to_member_function_t<Return, Class, Args...>> : public ArgumentInfoBase<Args...> {};