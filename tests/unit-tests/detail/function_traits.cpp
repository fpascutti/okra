
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <tuple>
#include <type_traits>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/detail/function_traits.h>

namespace {

int foo(const char*, double, float&);

struct widget
{
    double bar(const int&, void(*)(int), unsigned int*);
    float baz(std::tuple<char, short>) const;
    void(*fn_)(int, double);
};

struct gadget
{
  short operator()(const int&, const int&, double*, char(&)[5]);
};

}

TEST(function_traits, standard_function)
{
    typedef okra::detail::function_traits<decltype(foo)> traits;
    static_assert(std::is_same<int, typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 3, "Invalid number of arguments detected.");
    static_assert(std::is_same<const char*, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<double, typename std::tuple_element<1, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<float&, typename std::tuple_element<2, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}

TEST(function_traits, function_pointer)
{
    typedef okra::detail::function_traits<decltype(&foo)> traits;
    static_assert(std::is_same<int, typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 3, "Invalid number of arguments detected.");
    static_assert(std::is_same<const char*, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<double, typename std::tuple_element<1, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<float&, typename std::tuple_element<2, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}

TEST(function_traits, member_function)
{
    typedef okra::detail::function_traits<decltype(&widget::bar)> traits;
    static_assert(std::is_same<double, typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 4, "Invalid number of arguments detected.");
    static_assert(std::is_same<widget&, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<const int&, typename std::tuple_element<1, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<void(*)(int), typename std::tuple_element<2, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<unsigned int*, typename std::tuple_element<3, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}

TEST(function_traits, const_member_function)
{
    typedef okra::detail::function_traits<decltype(&widget::baz)> traits;
    static_assert(std::is_same<float, typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 2, "Invalid number of arguments detected.");
    static_assert(std::is_same<const widget&, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<std::tuple<char, short>, typename std::tuple_element<1, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}

TEST(function_traits, member_variable)
{
    typedef okra::detail::function_traits<decltype(&widget::fn_)> traits;
    static_assert(std::is_same<void(*)(int, double), typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 1, "Invalid number of arguments detected.");
    static_assert(std::is_same<widget&, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}

TEST(function_traits, functor_objects)
{
    typedef okra::detail::function_traits<gadget> traits;
    static_assert(std::is_same<short, typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 4, "Invalid number of arguments detected.");
    static_assert(std::is_same<const int&, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<const int&, typename std::tuple_element<1, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<double*, typename std::tuple_element<2, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<char(&)[5], typename std::tuple_element<3, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}

TEST(function_traits, lvalue_ref_functor_objects)
{
    typedef okra::detail::function_traits<gadget&> traits;
    static_assert(std::is_same<short, typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 4, "Invalid number of arguments detected.");
    static_assert(std::is_same<const int&, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<const int&, typename std::tuple_element<1, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<double*, typename std::tuple_element<2, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<char(&)[5], typename std::tuple_element<3, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}

TEST(function_traits, rvalue_ref_functor_objects)
{
    typedef okra::detail::function_traits<gadget&&> traits;
    static_assert(std::is_same<short, typename traits::result_type>::value, "Invalid return type detected.");
    static_assert(std::tuple_size<typename traits::arguments_type>::value == 4, "Invalid number of arguments detected.");
    static_assert(std::is_same<const int&, typename std::tuple_element<0, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<const int&, typename std::tuple_element<1, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<double*, typename std::tuple_element<2, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
    static_assert(std::is_same<char(&)[5], typename std::tuple_element<3, typename traits::arguments_type>::type>::value, "Invalid argument type detected.");
}
