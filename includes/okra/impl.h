#ifndef OKRA_IMPL_H
#define OKRA_IMPL_H

// Copyright Franck Pascutti 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <okra/detail/implementation.h>
#include <okra/detail/typed_registry.h>
#include <okra/re.h>

namespace okra {

namespace detail {

template<typename Char, typename RegexTraits, typename LFn, typename RFn>
typed_registry<Char, RegexTraits, LFn, RFn> operator|(implementation<Char, RegexTraits, LFn>&& left, implementation<Char, RegexTraits, RFn>&& right)
{
    typedef typed_registry<Char, RegexTraits, LFn, RFn> registry_type;
    return registry_type(std::move(left), std::move(right));
}

template<typename Char, typename RegexTraits, typename ImplFn, typename... RegFns>
typed_registry<Char, RegexTraits, RegFns..., ImplFn> operator|(typed_registry<Char, RegexTraits, RegFns...>&& reg, implementation<Char, RegexTraits, ImplFn>&& impl)
{
    typedef typed_registry<Char, RegexTraits, RegFns..., ImplFn> registry_type;
    return registry_type(std::move(reg), std::move(impl));
}

}

template<typename Char, typename RegexTraits, typename Fn>
detail::implementation<Char, RegexTraits, Fn> impl(re::basic_regex<Char, RegexTraits>&& regex, Fn&& fn)
{
    return detail::implementation<Char, RegexTraits, Fn>(std::move(regex), std::forward<Fn>(fn));
}

}

#endif // OKRA_IMPL_H
