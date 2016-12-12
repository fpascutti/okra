
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/scenario.h>
#include <okra/step.h>

namespace {

struct framework_mock
{
    MOCK_CONST_METHOD0(has_failed, bool());

    MOCK_CONST_METHOD1(undefined_step, void(const okra::step&));

    MOCK_CONST_METHOD1(before_scenario, void(const okra::scenario&));
    MOCK_CONST_METHOD1(after_scenario, void(const okra::scenario&));

    MOCK_CONST_METHOD1(before_step, void(const okra::step&));
    MOCK_CONST_METHOD1(after_step, void(const okra::step&));
};

struct registry_mock
{
    bool operator()(const std::string& s, framework_mock& fw) const { return this->call_op(s, fw); }
    MOCK_CONST_METHOD2(call_op, bool(const std::string&, framework_mock&));
};

}

TEST(scenario, narrow_constructor)
{
    okra::scenario s("I should have a perfect day");
    ASSERT_STREQ("I should have a perfect day", s.name().c_str()) << "'scenario' constructor did not properly set its name.";
}

TEST(scenario, wide_constructor)
{
    okra::wscenario s(L"I should have a perfect day");
    ASSERT_STREQ(L"I should have a perfect day", s.name().c_str()) << "'scenario' constructor did not properly set its name.";
}

TEST(scenario, no_steps)
{
    okra::scenario s("I should have a perfect day");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<registry_mock> reg;

    // check that before/after_scenario is called but no steps
    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).Times(0);
    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(scenario, multiple_steps)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<registry_mock> reg;

    // check that steps are called in order (and between before/after_scenario calls)
    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);
    EXPECT_CALL(reg, call_op(testing::StrEq("Given I wake up"), testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(reg, call_op(testing::StrEq("When I take a shower"), testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(reg, call_op(testing::StrEq("Then I feel better"), testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(scenario, multiple_steps_failure)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<registry_mock> reg;

    // check that steps are not called following a failure
    testing::InSequence seq;
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(true));
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).Times(0);

    s(fw, reg);
}

TEST(scenario, exception_in_before_scenario)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<registry_mock> reg;

    // check that steps are not called following an exception
    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::_)).WillOnce(testing::Throw(std::runtime_error("failed")));
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).Times(0);

    ASSERT_THROW(s(fw, reg), std::runtime_error) << "'scenario' call operator silently swallowed the exception.";
}

TEST(scenario, exception_when_invoking_step)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::NiceMock<framework_mock> fw;
    testing::NiceMock<registry_mock> reg;

    // // check that steps are not called following an exception
    testing::InSequence seq;
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).WillOnce(testing::Throw(std::runtime_error("failed")));
    EXPECT_CALL(reg, call_op(testing::_, testing::_)).Times(0);

    ASSERT_THROW(s(fw, reg), std::runtime_error) << "'scenario' call operator silently swallowed the exception.";
}

TEST(scenario, exception_in_after_scenario)
{
    // We put all test logic inside the death test block
    // so that mocks expectations can be set/used (as far as possible - i.e. mock destructor is not called).
    // If test fails to die then it probably means that an expectation is incorrect
    ASSERT_DEATH({
        okra::scenario s("I should have a perfect day");
        s.add_step("Given I wake up");
        s.add_step("When I take a shower");
        s.add_step("Then I feel better");

        testing::NiceMock<framework_mock> fw;
        testing::NiceMock<registry_mock> reg;
        ON_CALL(reg, call_op(testing::_, testing::_)).WillByDefault(testing::Return(true));

        testing::InSequence seq;
        EXPECT_CALL(fw, after_scenario(testing::Ref(s))).WillOnce(testing::Throw(std::runtime_error("failed")));

        s(fw, reg);

    }, "") << "'scenario' call operator should call 'terminate' in an exception is thrown in 'after_scenario'.";
}
