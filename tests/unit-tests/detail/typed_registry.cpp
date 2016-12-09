
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/impl.h>
#include <okra/re.h>

namespace {

struct invokable0
{
    void operator()(int n) const
    { return this->call_op(n); }
    MOCK_CONST_METHOD1(call_op, void(int));
};
struct invokable1
{
    void operator()(int n, const std::string& s0) const
    { return this->call_op(n, s0); }
    MOCK_CONST_METHOD2(call_op, void(int, const std::string&));
};
struct invokable2
{
    void operator()(int n, const std::string& s0, const std::string& s1) const
    { return this->call_op(n, s0, s1); }
    MOCK_CONST_METHOD3(call_op, void(int, const std::string&, const std::string&));
};

}

TEST(typed_registry, success)
{
    testing::NiceMock<invokable2> step0;
    testing::NiceMock<invokable1> step1;
    testing::NiceMock<invokable0> step2;

    auto reg = okra::impl(okra::re::regex("Given I (.*) up in the (.*)"), step0)
             | okra::impl(okra::re::regex("When I take a (.*)"), step1)
             | okra::impl(okra::re::regex("Then I feel better"), step2);

    {
        EXPECT_CALL(step0, call_op(42, testing::StrEq("wake"), testing::StrEq("morning"))).Times(1);
        EXPECT_CALL(step1, call_op(testing::_, testing::_)).Times(0);
        EXPECT_CALL(step2, call_op(testing::_)).Times(0);

        auto res = reg("Given I wake up in the morning", 42);
        ASSERT_TRUE(res) << "A successful call shoud return `true`.";
        testing::Mock::VerifyAndClearExpectations(&step0);
        testing::Mock::VerifyAndClearExpectations(&step1);
        testing::Mock::VerifyAndClearExpectations(&step2);
    }

    {
        EXPECT_CALL(step0, call_op(testing::_, testing::_, testing::_)).Times(0);
        EXPECT_CALL(step1, call_op(3, testing::StrEq("shower"))).Times(1);
        EXPECT_CALL(step2, call_op(testing::_)).Times(0);

        auto res = reg("When I take a shower", 3);
        ASSERT_TRUE(res) << "A successful call shoud return `true`.";
        testing::Mock::VerifyAndClearExpectations(&step0);
        testing::Mock::VerifyAndClearExpectations(&step1);
        testing::Mock::VerifyAndClearExpectations(&step2);
    }

    {
        EXPECT_CALL(step0, call_op(testing::_, testing::_, testing::_)).Times(0);
        EXPECT_CALL(step1, call_op(testing::_, testing::_)).Times(0);
        EXPECT_CALL(step2, call_op(1024)).Times(1);

        auto res = reg("Then I feel better", 1024);
        ASSERT_TRUE(res) << "A successful call shoud return `true`.";
        testing::Mock::VerifyAndClearExpectations(&step0);
        testing::Mock::VerifyAndClearExpectations(&step1);
        testing::Mock::VerifyAndClearExpectations(&step2);
    }
}

TEST(typed_registry, no_match)
{
    testing::NiceMock<invokable2> step0;
    testing::NiceMock<invokable1> step1;
    testing::NiceMock<invokable0> step2;

    auto reg = okra::impl(okra::re::regex("Given I (.*) up in the (.*)"), step0)
             | okra::impl(okra::re::regex("When I take a (.*)"), step1)
             | okra::impl(okra::re::regex("Then I feel better"), step2);

    EXPECT_CALL(step0, call_op(testing::_, testing::_, testing::_)).Times(0);
    EXPECT_CALL(step1, call_op(testing::_, testing::_)).Times(0);
    EXPECT_CALL(step2, call_op(testing::_)).Times(0);

    auto res = reg("Given I go to sleep in the evening", 42);
    ASSERT_FALSE(res) << "A failed find and call shoud return `false`.";
}
