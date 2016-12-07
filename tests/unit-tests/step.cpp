
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/step.h>

namespace {

struct framework_mock
{
    MOCK_CONST_METHOD1(undefined_step, void(const okra::step&));

    MOCK_CONST_METHOD1(before_step, void(const okra::step&));
    MOCK_CONST_METHOD1(after_step, void(const okra::step&));
};

struct impl_mock
{
    void operator()(const std::string& s, framework_mock& fw) const { return this->call_op(s, fw); }
    MOCK_CONST_METHOD2(call_op, void(const std::string&, framework_mock&));
};

struct registry_mock
{
    MOCK_CONST_METHOD1(find_impl, const impl_mock*(const std::string&));
};

}

TEST(step, narrow_constructor)
{
    okra::step s("Given I wake up");
    ASSERT_STREQ("Given I wake up", s.text().c_str()) << "'step' constructor did not properly set its text.";
}

TEST(step, wide_constructor)
{
    okra::wstep s(L"Given I wake up");
    ASSERT_STREQ(L"Given I wake up", s.text().c_str()) << "'step' constructor did not properly set its text.";
}

TEST(step, success)
{
    okra::step s("Given I wake up");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<impl_mock> impl;
    testing::NiceMock<registry_mock> reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_step(testing::Ref(s))).Times(1);
    EXPECT_CALL(reg, find_impl(testing::StrEq("Given I wake up"))).WillOnce(testing::Return(&impl));
    EXPECT_CALL(impl, call_op(testing::StrEq("Given I wake up"), testing::Ref(fw))).Times(1);
    EXPECT_CALL(fw, after_step(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(step, undefined_step)
{
    okra::step s("Given I wake up");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<registry_mock> reg;

    testing::InSequence seq;
    EXPECT_CALL(reg, find_impl(testing::StrEq("Given I wake up"))).WillOnce(testing::Return(nullptr));
    EXPECT_CALL(fw, undefined_step(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(step, exception_in_before_step)
{
    okra::step s("Given I wake up");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<registry_mock> reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_step(testing::_)).WillOnce(testing::Throw(std::runtime_error("failed")));

    ASSERT_THROW(s(fw, reg), std::runtime_error) << "'step' call operator silently swallowed the exception.";
}

TEST(step, exception_in_after_step)
{
    // We put all test logic inside the death test block
    // so that mocks expectations can be set/used (as far as possible - i.e. mock destructor is not called).
    // If test fails to die then it probably means that an expectation is incorrect
    ASSERT_DEATH({
        okra::step s("Given I wake up");

        testing::NiceMock<framework_mock> fw;
        testing::NiceMock<impl_mock> impl;
        testing::NiceMock<registry_mock> reg;
        ON_CALL(reg, find_impl(testing::_)).WillByDefault(testing::Return(&impl));

        testing::InSequence seq;
        EXPECT_CALL(fw, after_step(testing::_)).WillOnce(testing::Throw(std::runtime_error("failed")));

        s(fw, reg);
    }, "") << "'step' call operator should call 'terminate' in an exception is thrown in 'after_step'.";
}
