
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <okra/step.h>

namespace {

struct framework_mock
{
    MOCK_CONST_METHOD1(before_step, void(const okra::step&));
    MOCK_CONST_METHOD1(after_step, void(const okra::step&));
};
struct registry_mock
{
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

TEST(step, call)
{
    okra::step s("Given I wake up");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_step(testing::Ref(s))).Times(1);
    EXPECT_CALL(fw, after_step(testing::Ref(s))).Times(1);

    s(fw, reg);
}

TEST(step, exception_in_before_step)
{
    okra::step s("Given I wake up");

    testing::StrictMock<framework_mock> fw;
    registry_mock reg;

    testing::InSequence seq;
    EXPECT_CALL(fw, before_step(testing::Ref(s))).WillOnce(testing::Throw(std::runtime_error("failed")));

    ASSERT_THROW(s(fw, reg), std::runtime_error) << "'step' call operator silently swallowed the exception.";
}

TEST(step, exception_in_after_step)
{
    // We put all test logic inside the death test block
    // so that mocks expectations can be set/used (as far as possible - i.e. mock destructor is not called).
    // If test fails to die then it probably means that an expectation is incorrect
    ASSERT_DEATH({
        okra::step s("Given I wake up");

        testing::StrictMock<framework_mock> fw;
        registry_mock reg;

        testing::InSequence seq;
        EXPECT_CALL(fw, before_step(testing::Ref(s))).Times(1);
        EXPECT_CALL(fw, after_step(testing::Ref(s))).WillOnce(testing::Throw(std::runtime_error("failed")));

        s(fw, reg);
    }, "") << "'step' call operator should call 'terminate' in an exception is thrown in 'after_step'.";
}
