
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

int main(int args_count, char* args[])
{
    ::testing::InitGoogleTest(&args_count, args);
    return RUN_ALL_TESTS();
}
