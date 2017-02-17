#ifndef OKRA_CATCH_FRAMEWORK_H
#define OKRA_CATCH_FRAMEWORK_H

// Copyright Franck Pascutti 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// We are using internals of Catch because there are no official ways
// to do what we need.
// This was tested only with Catch 1.5.0 but *may* work with other versions.

#include <memory>
#include <string>
#include <catch.hpp>
#include <okra/detail/wstring_convert.h>
#include <okra/config.h>
#include <okra/scenario.h>
#include <okra/step.h>

namespace okra { namespace catch_ {

class framework
{
public:
    bool has_failed() const
    {
		return false; // catch throws if a fatal failure is detected so we do not need that
    }

public:
    template<typename Char, typename Traits>
    void undefined_step(const okra::basic_step<Char, Traits>& s)
    {
		// No equivalent of `FAIL` exists that takes file and line information.
		Catch::SourceLineInfo info(s.file().c_str(), s.line());
		Catch::ResultBuilder result("UNDEFINED STEP", info, to_bytes(s.text()).c_str(), Catch::ResultDisposition::Normal);
		result.captureResult(Catch::ResultWas::ExpressionFailed);
		INTERNAL_CATCH_REACT(result)
    }

public:
    template<typename Char, typename Traits>
    void before_scenario(const okra::basic_scenario<Char, Traits>& s)
    {
        current_scenario_ = make_scoped_trace(s.file().c_str(), s.line(), "BEFORE SCENARIO", s.name());
    }
    template<typename Char, typename Traits>
    void after_scenario(const okra::basic_scenario<Char, Traits>&)
    {
        current_scenario_.reset();
    }
    template<typename Char, typename Traits>
    void before_step(const okra::basic_step<Char, Traits>& s)
    {
        current_step_ = make_scoped_trace(s.file().c_str(), s.line(), "BEFORE STEP", s.text());
    }
    template<typename Char, typename Traits>
    void after_step(const okra::basic_step<Char, Traits>&)
    {
        current_step_.reset();
    }

private:
    template<typename Char, typename Traits>
    static std::unique_ptr<Catch::ScopedMessage> make_scoped_trace(const char* file, int line, const char* macro, const std::basic_string<Char, Traits>& msg)
    {
		// No equivalent of `INFO` exists that take file and line information.
		Catch::SourceLineInfo info(file, line);
		return std::unique_ptr<Catch::ScopedMessage>(new Catch::ScopedMessage(Catch::MessageBuilder(macro, info, Catch::ResultWas::Info) << info << ": " << to_bytes(msg)));
	}

private:
	template<typename Traits>
	static const std::basic_string<char, Traits>& to_bytes(const std::basic_string<char, Traits>& txt)
	{
		return txt;
	}
	template<typename Traits>
	static std::string to_bytes(const std::basic_string<wchar_t, Traits>& txt)
	{
		return okra::detail::wstring_convert::to_bytes(txt);
	}

private:
	std::unique_ptr<Catch::ScopedMessage> current_scenario_;
	std::unique_ptr<Catch::ScopedMessage> current_step_;
};

} }

#endif // OKRA_CATCH_FRAMEWORK_H
