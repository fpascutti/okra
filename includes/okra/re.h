#ifndef OKRA_RE_H
#define OKRA_RE_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if OKRA_CONFIG_USEBOOSTREGEX

#include <boost/regex.hpp>

namespace okra {
    namespace re = ::boost;
}

#else

#include <regex>

namespace okra {
    namespace re = ::std;
}

#endif // OKRA_CONFIG_USEBOOSTREGEX

#endif // OKRA_RE_H
