
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <memory>
#include <string>
#include <gtest/gtest.h>
#include <okra/ignore.h>
#include <okra/impl.h>
#include <okra/re.h>
#include <okra/scenario.h>

namespace {

class framework
{
public:
    bool has_failed() const { return testing::Test::HasFatalFailure(); }

public:
    void undefined_step(const okra::step& s)
    {
        FAIL() << "No implementation found for step '" << s.text() << "'.";
    }

public:
    void before_scenario(const okra::scenario& s)
    {
        current_scenario_ = make_scoped_trace(s.file().c_str(), s.line(), s.name().c_str());
    }
    void after_scenario(const okra::scenario&)
    {
        current_scenario_.reset();
    }
    void before_step(const okra::step& s)
    {
        current_step_ = make_scoped_trace(__FILE__, __LINE__, s.text().c_str());
    }
    void after_step(const okra::step&)
    {
        current_step_.reset();
    }

private:
    static std::unique_ptr<testing::internal::ScopedTrace> make_scoped_trace(const char* file, int line, const char* msg)
    {
        return std::unique_ptr<testing::internal::ScopedTrace>(new testing::internal::ScopedTrace(file, line, testing::Message() << msg));
    }

private:
    std::unique_ptr<testing::internal::ScopedTrace> current_scenario_;
    std::unique_ptr<testing::internal::ScopedTrace> current_step_;
};
framework framework__;

unsigned long long factorial(unsigned long long)
{
    return 1ull;
}

auto registry__ = okra::impl(okra::re::regex("Given I have the number (\\d+)"), [](framework&, unsigned long long& input, okra::ignore, const std::string& arg) { input = std::stoull(arg); })
                | okra::impl(okra::re::regex("When I compute its factorial"), [](framework&, unsigned long long input, unsigned long long& result) { result = factorial(input); })
                | okra::impl(okra::re::regex("Then I see the number (\\d+)"), [](framework&, unsigned long long input, unsigned long long result, const std::string& arg) { auto exp = std::stoull(arg); ASSERT_EQ(exp, result) << "Invalid factorial computed for '" << input << "'."; });
}

TEST(factorial, factorial_of_0)
{
    okra::scenario s("Factorial of 0", __FILE__, __LINE__);
    s.add_step("Given I have the number 0");
    s.add_step("When I compute its factorial");
    s.add_step("Then I see the number 1");

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

TEST(factorial, factorial_of_1)
{
    okra::scenario s("Factorial of 1", __FILE__, __LINE__);
    s.add_step("Given I have the number 1");
    s.add_step("When I compute its factorial");
    s.add_step("Then I see the number 1");

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

TEST(factorial, factorial_of_2)
{
    okra::scenario s("Factorial of 2", __FILE__, __LINE__);
    s.add_step("Given I have the number 2");
    s.add_step("When I compute its factorial");
    s.add_step("Then I see the number 2");

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

TEST(factorial, factorial_of_3)
{
    okra::scenario s("Factorial of 3", __FILE__, __LINE__);
    s.add_step("Given I have the number 3");
    s.add_step("When I compute its factorial");
    s.add_step("Then I see the number 6");

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}
