#ifndef OKRA_DETAIL_FIND_AND_CALL_H
#define OKRA_DETAIL_FIND_AND_CALL_H

// Copyright Franck Pascutti 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <tuple>
#include <utility>

namespace okra { namespace detail {

template<std::size_t Index>
struct finder_and_caller
{
    template<typename... Ts, typename... Args>
    bool operator()(const std::tuple<Ts...>& t, Args&&... args) const
    {
        if(std::get<Index - 1>(t)(std::forward<Args>(args)...))
            return true;
        return finder_and_caller<Index - 1>()(t, std::forward<Args>(args)...);
    }
};
template<>
struct finder_and_caller<0>
{
    template<typename... Ts, typename... Args>
    bool operator()(const std::tuple<Ts...>&, Args&&...) const
    {
        return false;
    }
};

template<typename... Ts, typename... Args>
bool find_and_call(const std::tuple<Ts...>& t, Args&&... args)
{
    return finder_and_caller<sizeof...(Ts)>()(t, std::forward<Args>(args)...);
}

} }

#endif // OKRA_DETAIL_FIND_AND_CALL_H
