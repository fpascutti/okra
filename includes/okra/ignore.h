#ifndef OKRA_IGNORE_H
#define OKRA_IGNORE_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

namespace okra {

class ignore
{
public:
    template<typename T>
    ignore(T&&)
    { }
};


}

#endif // OKRA_IGNORE_H
