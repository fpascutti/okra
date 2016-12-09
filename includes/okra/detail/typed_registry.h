#ifndef OKRA_DETAIL_TYPED_REGISTRY_H
#define OKRA_DETAIL_TYPED_REGISTRY_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <utility>
#include <okra/detail/find_and_call.h>
#include <okra/detail/implementation.h>
#include <okra/re.h>

namespace okra { namespace detail {

template<typename Char, typename RegexTraits, typename... Fns>
class typed_registry
{
private:
    template<typename C, typename RT, typename... Fs> friend class typed_registry;

    typedef re::basic_regex<Char, RegexTraits> regex_type;
    typedef typename RegexTraits::string_type string_type;
    typedef std::tuple<implementation<Char, RegexTraits, Fns>...> impls_tuple_type;

public:
    template<typename LFn, typename RFn>
    typed_registry(implementation<Char, RegexTraits, LFn>&& left, implementation<Char, RegexTraits, RFn>&& right)
        : impls_(std::make_tuple(std::move(left), std::move(right)))
    { }

    template<typename ImplFn, typename... RegFns>
    typed_registry(typed_registry<Char, RegexTraits, RegFns...>&& reg, implementation<Char, RegexTraits, ImplFn>&& impl)
        : impls_(std::tuple_cat(std::move(reg.impls_), std::make_tuple(std::move(impl))))
    { }

    template<typename... Args>
    bool operator()(const string_type& step, Args&&... args) const
    {
        return find_and_call(impls_, step, std::forward<Args>(args)...);
    }

private:
    impls_tuple_type impls_;
};

} }

#endif // OKRA_DETAIL_TYPED_REGISTRY_H
