
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <okra/scenario.h>

int main()
{
    okra::scenario s("A package should be correctly created");
    s.add_step("Given I am making modifications to an okra package");
    s.add_step("When I am testing it");
    s.add_step("Then this file should compiler successfully");
    return 0;
}
