#ifndef OKRA_DETAIL_INVOKE_H
#define OKRA_DETAIL_INVOKE_H

// Copyright Franck Pascutti 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <tuple>
#include <utility>
#include <okra/detail/function_traits.h>

namespace okra { namespace detail {

template<std::size_t Index, std::size_t Processed, std::size_t Arity>
struct invoker
{
    static_assert(Processed < Arity, "Invoking should stop once `Processed == Arity`");
    static_assert(Processed >= Index, "Number of `Processed` arguments should be at least equal to the number of matches already extracted.");
    template<typename Matches, typename Fn, typename... Args>
    void operator()(const Matches& matches, Fn&& fn, Args&&... args) const
    {
        return invoker<Index + 1, Processed + 1, Arity>()(matches, std::forward<Fn>(fn), std::forward<Args>(args)..., matches[Index + 1]);
    }
};
template<std::size_t Index, std::size_t Processed>
struct invoker<Index, Processed, Processed>
{
    static_assert(Processed >= Index, "Number of `Processed` arguments should be at least equal to the number of matches already extracted.");
    template<typename Matches, typename Fn, typename... Args>
    void operator()(const Matches&, Fn&& fn, Args&&... args) const
    {
        return std::forward<Fn>(fn)(std::forward<Args>(args)...);
    }
};

template<typename Matches, typename Fn, typename... Args>
void invoke(const Matches& matches, Fn&& fn, Args&&... args)
{
    return invoker<0, sizeof...(Args), std::tuple_size<typename function_traits<Fn>::arguments_type>::value>()(matches, std::forward<Fn>(fn), std::forward<Args>(args)...);
}

} }


#endif // OKRA_DETAIL_INVOKE_H
