#ifndef OKRA_DETAIL_FUNCTION_TRAITS_H
#define OKRA_DETAIL_FUNCTION_TRAITS_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>

namespace okra { namespace detail {

//
// `function_traits` class gathers information about a function.
// It is used by **okra** to get the arity (number of arguments) of any function
// so that it knows how many regex `sub_match` to extract before invoking it.
//
// Code is shamelessly stolen from [Functional C++ | Function Traits](https://functionalcpp.wordpress.com/2013/08/05/function-traits/).
//

template<typename Fn>
struct function_traits
{
private:
    typedef function_traits<decltype(&Fn::operator())> call_type;
    template<typename T>
    struct remove_first;
    template<typename First, typename... Rest>
    struct remove_first<std::tuple<First, Rest...> >
    {
        typedef std::tuple<Rest...> type;
    };
public:
    typedef typename call_type::result_type result_type;
    typedef typename remove_first<typename call_type::arguments_type>::type arguments_type;
};
template<typename Fn>
struct function_traits<Fn&> : function_traits<Fn>
{ };
template<typename Fn>
struct function_traits<Fn&&> : function_traits<Fn>
{ };

template<typename R, typename... Args>
struct function_traits<R(Args...)>
{
    typedef R result_type;
    typedef std::tuple<Args...> arguments_type;
};
template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)>
{ };

template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...)> : function_traits<R(C&, Args...)>
{ };
template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) const> : function_traits<R(const C&, Args...)>
{ };
template<class C, class R>
struct function_traits<R(C::*)> : function_traits<R(C&)>
{ };

} }

#endif // OKRA_DETAIL_FUNCTION_TRAITS_H
