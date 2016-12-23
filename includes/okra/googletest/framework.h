#ifndef OKRA_GOOGLETEST_FRAMEWORK_H
#define OKRA_GOOGLETEST_FRAMEWORK_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// We are using internals of GoogleTest because there are no official ways
// to do what we need.
// This was tested only with GoogleTest 1.8.0 but *may* work with other versions.

#include <locale>
#include <memory>
#include <string>
#include <utility>
#include <gtest/gtest.h>
#include <okra/config.h>
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
    template<typename Char, typename Traits>
    void undefined_step(const okra::basic_step<Char, Traits>& s)
    {
        // No equivalent of `ADD_FAILURE_AT` exists that creates a fatal failure.
        GTEST_MESSAGE_AT_(s.file().c_str(), s.line(), "Undefined step", testing::TestPartResult::kFatalFailure) << "No implementation found for step '" << to_bytes(s.text()) << "'.";
    }

public:
    template<typename Char, typename Traits>
    void before_scenario(const okra::basic_scenario<Char, Traits>& s)
    {
        current_scenario_ = make_scoped_trace(s.file().c_str(), s.line(), s.name());
    }
    template<typename Char, typename Traits>
    void after_scenario(const okra::basic_scenario<Char, Traits>&)
    {
        current_scenario_.reset();
    }
    template<typename Char, typename Traits>
    void before_step(const okra::basic_step<Char, Traits>& s)
    {
        current_step_ = make_scoped_trace(s.file().c_str(), s.line(), s.text());
    }
    template<typename Char, typename Traits>
    void after_step(const okra::basic_step<Char, Traits>&)
    {
        current_step_.reset();
    }

private:
    template<typename Char, typename Traits>
    static std::unique_ptr<testing::internal::ScopedTrace> make_scoped_trace(const char* file, int line, const std::basic_string<Char, Traits>& msg)
    {
        // No equivalent of `SCOPED_TRACE` exists where we can specify the file name and line number.
        return std::unique_ptr<testing::internal::ScopedTrace>(new testing::internal::ScopedTrace(file, line, testing::Message() << to_bytes(msg)));
    }

private:
    class codecvt
        : public std::codecvt<wchar_t, char, std::mbstate_t>
    {
    private:
        typedef std::codecvt<wchar_t, char, std::mbstate_t> base_type;
    public:
        template<typename... Args>
        codecvt(Args&&... args)
            : base_type(std::forward<Args>(args)...)
        { }
        virtual ~codecvt() OKRA_OVERRIDE
        { }
    };

    template<typename Traits>
    static const std::basic_string<char, Traits>& to_bytes(const std::basic_string<char, Traits>& txt)
    {
        return txt;
    }
    template<typename Traits>
    static std::string to_bytes(const std::basic_string<wchar_t, Traits>& txt)
    {
        std::wstring_convert<codecvt> converter;
        return converter.to_bytes(txt.c_str());
    }

private:
    std::unique_ptr<testing::internal::ScopedTrace> current_scenario_;
    std::unique_ptr<testing::internal::ScopedTrace> current_step_;
};

} }

#endif // OKRA_GOOGLETEST_FRAMEWORK_H
