
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <memory>
#include <gtest/gtest.h>
#include <okra/scenario.h>

namespace {

class framework
{
public:
    bool has_failed() const { return testing::Test::HasFatalFailure(); }

    void before_scenario(const okra::scenario& s)
    {
        current_scenario_ = make_scoped_trace(__FILE__, __LINE__, s.name().c_str());
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
        FAIL() << "fake error";
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

struct registry {};
registry registry__;

}

TEST(factorial, factorial_of_0)
{
    okra::scenario s("Factorial of 0");
    s.add_step("Given I have the number 0");
    s.add_step("When I compute its factorial");
    s.add_step("Then I see the number 1");
    s(framework__, registry__);
}
