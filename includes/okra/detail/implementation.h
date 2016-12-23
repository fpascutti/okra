#ifndef OKRA_DETAIL_IMPLEMENTATION_H
#define OKRA_DETAIL_IMPLEMENTATION_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <type_traits>
#include <utility>
#include <okra/detail/function_traits.h>
#include <okra/detail/invoke.h>
#include <okra/re.h>

namespace okra { namespace detail {

template<typename Char, typename RegexTraits, typename Fn>
class implementation
{
private:
    typedef re::basic_regex<Char, RegexTraits> regex_type;
    typedef typename RegexTraits::string_type string_type;
    typedef Fn function_type;

public:
    implementation(regex_type&& regex, typename std::remove_reference<function_type>::type&& fn)
        : re_(std::move(regex)),
          fn_(std::move(fn))
    { }

    implementation(regex_type&& regex, typename std::remove_reference<function_type>::type& fn)
        : re_(std::move(regex)),
          fn_(fn)
    { }

    template<typename... Args>
    bool operator()(const string_type& step, Args&&... args) const
    {
        typedef re::match_results<typename string_type::const_iterator> matches_type;
        matches_type matches;
        if(re::regex_search(step, matches, re_))
        {
            if((sizeof...(Args) + matches.size() - 1) == std::tuple_size<typename function_traits<function_type>::arguments_type>::value)
            {
                invoke(matches, fn_, std::forward<Args>(args)...);
                return true;
            }
        }
        return false;
    }
private:
    regex_type re_;
    function_type fn_;
};

} }

#endif // OKRA_DETAIL_IMPLEMENTATION_H
