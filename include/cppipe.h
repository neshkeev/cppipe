#pragma once

#include <functional>
#include <concepts>
#include <variant>
#include <string_view>
#include <any>

namespace cppipe {

  using Error = std::string_view;

  template<class T, class E = Error>
  using Result = std::variant<E, T>;

  template<class T, class F>
  concept FlatMap = std::invocable<F, T> && requires(T t, F f) {
    { std::invoke(std::forward<F>(f), std::forward<T>(t)) } -> std::convertible_to<Result<std::any>>;
  };

  template<class T, class E = Error, class F>
    requires FlatMap<T, F>
  [[nodiscard]] auto operator>>(const Result<T, E>& a, F fun) noexcept
    -> decltype(fun(std::get<1>(a)))
  {
    using ResultType = decltype(fun(std::get<1>(a)));
    const auto visitor = [&fun](auto&& arg) -> ResultType {
      using A = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<A, std::decay_t<T>>) {
        return fun(std::forward<decltype(arg)>(arg));
      }
      else {
        enum Tag { Err = 0, Val };
        using ValueType = std::variant_alternative_t<Tag::Val, ResultType>;
        using ErrorType = std::variant_alternative_t<Tag::Err, ResultType>;
        return Result<ValueType, ErrorType> { std::in_place_index<0>, std::forward<decltype(arg)>(arg) };
      }
    };

    return std::visit(visitor, std::forward<decltype(a)>(a));
  }

  template<class T, class E = Error, class F>
    requires FlatMap<T, F>
  [[nodiscard]] auto operator>>(Result<T>&& a, F fun) noexcept
    -> decltype(fun(std::get<1>(a)))
  {
    using ResultType = decltype(fun(std::get<1>(a)));
    const auto visitor = [&fun](auto&& arg) -> decltype(fun(std::get<1>(a))) {
      using A = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<A, std::decay_t<T>>) {
        return fun(std::forward<decltype(arg)>(arg));
      }
      else {
        enum Tag { Err = 0, Val };
        using ValueType = std::variant_alternative_t<Tag::Val, ResultType>;
        using ErrorType = std::variant_alternative_t<Tag::Err, ResultType>;
        return Result<ValueType, ErrorType> { std::in_place_index<0>, std::forward<decltype(arg)>(arg) };
      }
    };

    return std::visit(visitor, std::move(a));
  }

  template<class T, class E, class Success, class Failure>
  concept ResultHandler =
    std::invocable<Success, T> &&
    std::invocable<Failure, E> &&
    requires(T a, Success s, Failure f, E e) {
      { std::invoke(std::forward<Success>(s), std::forward<T>(a)) } ->
        std::common_with<decltype(std::invoke(std::forward<Failure>(f), std::forward<E>(e)))>;
  };

  template<class T, class E = Error, class Success, class Failure>
    requires ResultHandler<T, E, Success, Failure>
  [[nodiscard]] auto run(Result<T>& a, Success s, Failure f) noexcept
    -> std::common_type_t<decltype(s(std::get<1>(a))), decltype(f(std::get<0>(a)))>
  {
    using Ret = std::common_type_t<decltype(s(std::get<1>(a))), decltype(f(std::get<0>(a)))>;

    const auto visitor = [&s, &f](auto&& arg) -> Ret
    {
      using A = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<A, std::decay_t<T>>) {
        return s(std::forward<decltype(arg)>(arg));
      }
      else {
        auto err = static_cast<E>(std::forward<decltype(arg)>(arg));
        return f(err);
      }
    };

    return std::visit(visitor, std::forward<decltype(a)>(a));
  }

}
