#ifndef OKRA_GOOGLETEST_FRAMEWORK_H
#define OKRA_GOOGLETEST_FRAMEWORK_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// We are using internals of GoogleTest because there are no official ways
// to do what we need.
// This was tested only with GoogleTest 1.8.0 but *may* work with other versions.

#include <memory>
#include <gtest/gtest.h>
#include <okra/scenario.h>
#include <okra/step.h>

namespace okra { namespace googletest {

class framework
{
public:
    bool has_failed() const
    {
        return testing::Test::HasFatalFailure();
    }

public:
    void undefined_step(const okra::step& s)
    {
        // No equivalent of `ADD_FAILURE_AT` exists that creates a fatal failure.
        GTEST_MESSAGE_AT_(s.file().c_str(), s.line(), "Undefined step", testing::TestPartResult::kFatalFailure) << "No implementation found for step '" << s.text() << "'.";
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
        current_step_ = make_scoped_trace(s.file().c_str(), s.line(), s.text().c_str());
    }
    void after_step(const okra::step&)
    {
        current_step_.reset();
    }

private:
    static std::unique_ptr<testing::internal::ScopedTrace> make_scoped_trace(const char* file, int line, const char* msg)
    {
        // No equivalent of `SCOPED_TRACE` exists where we can specify the file name and line number.
        return std::unique_ptr<testing::internal::ScopedTrace>(new testing::internal::ScopedTrace(file, line, testing::Message() << msg));
    }

private:
    std::unique_ptr<testing::internal::ScopedTrace> current_scenario_;
    std::unique_ptr<testing::internal::ScopedTrace> current_step_;
};

} }

#endif // OKRA_GOOGLETEST_FRAMEWORK_H
