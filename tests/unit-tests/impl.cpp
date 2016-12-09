
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/impl.h>
#include <okra/re.h>

namespace {

struct invokable
{
    void operator()(int n, double& d, const std::string& s0, const std::string& s1) const
    { return this->call_op(n, d, s0, s1); }
    MOCK_CONST_METHOD4(call_op, void(int, double&, const std::string&, const std::string&));
};

}

TEST(impl, matched)
{
    double d = 3.14;

    testing::NiceMock<invokable> fn;
    EXPECT_CALL(fn, call_op(42, testing::Ref(d), testing::StrEq("wake"), testing::StrEq("morning"))).Times(1);

    auto i = okra::impl(okra::re::regex("Given I (.*) up in the (.*)"), fn);
    auto res = i("Given I wake up in the morning", 42, d);
    EXPECT_TRUE(res) << "A successful call should return `true`.";
}

TEST(impl, not_matched)
{
    double d = 3.14;

    testing::NiceMock<invokable> fn;
    EXPECT_CALL(fn, call_op(testing::_, testing::_, testing::_, testing::_)).Times(0);

    auto i = okra::impl(okra::re::regex("When I (.*) up in the (.*)"), fn);
    auto res = i("Given I wake up in the morning", 42, d);
    EXPECT_FALSE(res) << "A failed match should return `false`.";
}

TEST(impl, too_few_matches)
{
    double d = 3.14;

    testing::NiceMock<invokable> fn;
    EXPECT_CALL(fn, call_op(testing::_, testing::_, testing::_, testing::_)).Times(0);

    auto i = okra::impl(okra::re::regex("Given I .* up in the (.*)"), fn);
    auto res = i("Given I wake up in the morning", 42, d);
    EXPECT_FALSE(res) << "A failed match should return `false`.";
}

TEST(impl, too_many_matches)
{
    double d = 3.14;

    testing::NiceMock<invokable> fn;
    EXPECT_CALL(fn, call_op(testing::_, testing::_, testing::_, testing::_)).Times(0);

    auto i = okra::impl(okra::re::regex("Given I (.*) up in the (.*) (.*)"), fn);
    auto res = i("Given I wake up in the morning happy", 42, d);
    EXPECT_FALSE(res) << "A failed match should return `false`.";
}

TEST(impl, partial_match)
{
    double d = 3.14;

    testing::NiceMock<invokable> fn;
    EXPECT_CALL(fn, call_op(42, testing::Ref(d), testing::StrEq("wake"), testing::StrEq("morning"))).Times(1);

    auto i = okra::impl(okra::re::regex("I (.*) up in the ([^\\s]*)"), fn);
    auto res = i("I wake up in the morning happy", 42, d);
    EXPECT_TRUE(res) << "A successful call should return `true`.";
}
