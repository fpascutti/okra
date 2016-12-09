#ifndef OKRA_IMPL_H
#define OKRA_IMPL_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <okra/detail/implementation.h>
#include <okra/re.h>

namespace okra {

template<typename Char, typename RegexTraits, typename Fn>
detail::implementation<Char, RegexTraits, Fn> impl(re::basic_regex<Char, RegexTraits>&& regex, Fn&& fn)
{
    return detail::implementation<Char, RegexTraits, Fn>(std::move(regex), std::forward<Fn>(fn));
}

}

#endif // OKRA_IMPL_H
