
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/scenario.h>

namespace {

struct framework_mock
{
    MOCK_CONST_METHOD0(has_failed, bool());

    MOCK_CONST_METHOD1(before_scenario, void(const okra::scenario&));
    MOCK_CONST_METHOD1(after_scenario, void(const okra::scenario&));

    MOCK_CONST_METHOD1(before_step, void(const okra::step&));
    MOCK_CONST_METHOD1(after_step, void(const okra::step&));
};
struct registry_mock
{
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

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);
    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(scenario, single_step_success)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);

    EXPECT_CALL(fw, before_step(testing::Property(&okra::step::text, testing::StrEq("Given I wake up")))).Times(1);
    EXPECT_CALL(fw, after_step(testing::Property(&okra::step::text, testing::StrEq("Given I wake up")))).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));

    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(scenario, single_step_failure)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);

    EXPECT_CALL(fw, before_step(testing::_)).Times(1);
    EXPECT_CALL(fw, after_step(testing::_)).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(true));

    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(scenario, multiple_steps_success)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);

    EXPECT_CALL(fw, before_step(testing::Property(&okra::step::text, testing::StrEq("Given I wake up")))).Times(1);
    EXPECT_CALL(fw, after_step(testing::Property(&okra::step::text, testing::StrEq("Given I wake up")))).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));

    EXPECT_CALL(fw, before_step(testing::Property(&okra::step::text, testing::StrEq("When I take a shower")))).Times(1);
    EXPECT_CALL(fw, after_step(testing::Property(&okra::step::text, testing::StrEq("When I take a shower")))).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));

    EXPECT_CALL(fw, before_step(testing::Property(&okra::step::text, testing::StrEq("Then I feel better")))).Times(1);
    EXPECT_CALL(fw, after_step(testing::Property(&okra::step::text, testing::StrEq("Then I feel better")))).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));

    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(scenario, multiple_steps_failure)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);

    EXPECT_CALL(fw, before_step(testing::_)).Times(1);
    EXPECT_CALL(fw, after_step(testing::_)).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));

    EXPECT_CALL(fw, before_step(testing::_)).Times(1);
    EXPECT_CALL(fw, after_step(testing::_)).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(true));

    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(scenario, exception_in_before_scenario)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).WillOnce(testing::Throw(std::runtime_error("failed")));

    ASSERT_THROW(s(fw, reg), std::runtime_error) << "'scenario' call operator silently swallowed the exception.";
}

TEST(scenario, exception_when_invoking_step)
{
    okra::scenario s("I should have a perfect day");
    s.add_step("Given I wake up");
    s.add_step("When I take a shower");
    s.add_step("Then I feel better");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);

    EXPECT_CALL(fw, before_step(testing::_)).Times(1);
    EXPECT_CALL(fw, after_step(testing::_)).Times(1);
    EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));

    EXPECT_CALL(fw, before_step(testing::_)).WillOnce(testing::Throw(std::runtime_error("failed")));

    EXPECT_CALL(fw, after_scenario(testing::Ref(s))).Times(1);

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

        testing::StrictMock<framework_mock> fw;
        registry_mock reg;

        testing::InSequence seq;
        EXPECT_CALL(fw, before_scenario(testing::Ref(s))).Times(1);

        EXPECT_CALL(fw, before_step(testing::_)).Times(1);
        EXPECT_CALL(fw, after_step(testing::_)).Times(1);
        EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(false));

        EXPECT_CALL(fw, before_step(testing::_)).Times(1);
        EXPECT_CALL(fw, after_step(testing::_)).Times(1);
        EXPECT_CALL(fw, has_failed()).WillOnce(testing::Return(true));

        EXPECT_CALL(fw, after_scenario(testing::Ref(s))).WillOnce(testing::Throw(std::runtime_error("failed")));

        s(fw, reg);

    }, "") << "'scenario' call operator should call 'terminate' in an exception is thrown in 'after_scenario'.";
}
