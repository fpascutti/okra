
// Copyright Franck Pascutti 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/detail/invoke.h>
#include <okra/re.h>

namespace {

struct invokable
{
    void operator()(int n, double& d, const std::string& s0, const std::string& s1) const
    { return this->call_op(n, d, s0, s1); }
    MOCK_CONST_METHOD4(call_op, void(int, double&, const std::string&, const std::string&));
};

}

TEST(invoke, exact_number_of_matches)
{
    double d = 3.14;

    std::string input("Given I wake up in the morning");
    okra::re::smatch matches;
    auto match_success = okra::re::regex_match(input, matches, okra::re::regex("Given I (.*) up in the (.*)"));
    ASSERT_TRUE(match_success) << "Regular expression match should have succeeded.";

    testing::StrictMock<invokable> fn;
    EXPECT_CALL(fn, call_op(42, testing::Ref(d), testing::StrEq("wake"), testing::StrEq("morning"))).Times(1);

    okra::detail::invoke(matches, fn, 42, d);
}

TEST(invoke, too_few_matches)
{
    double d = 3.14;

    std::string input("Given I wake up in the morning");
    okra::re::smatch matches;
    auto match_success = okra::re::regex_match(input, matches, okra::re::regex("Given I .* up in the .*"));
    ASSERT_TRUE(match_success) << "Regular expression match should have succeeded.";

    testing::StrictMock<invokable> fn;
    EXPECT_CALL(fn, call_op(42, testing::Ref(d), testing::StrEq(""), testing::StrEq(""))).Times(1);

    okra::detail::invoke(matches, fn, 42, d);
}

TEST(invoke, too_many_matches)
{
    double d = 3.14;

    std::string input("Given I wake up in the morning happy");
    okra::re::smatch matches;
    auto match_success = okra::re::regex_match(input, matches, okra::re::regex("Given I (.*) up in the (.*) (.*)"));
    ASSERT_TRUE(match_success) << "Regular expression match should have succeeded.";

    testing::StrictMock<invokable> fn;
    EXPECT_CALL(fn, call_op(42, testing::Ref(d), testing::StrEq("wake"), testing::StrEq("morning"))).Times(1);

    okra::detail::invoke(matches, fn, 42, d);
}

