#ifndef OKRA_SCENARIO_H
#define OKRA_SCENARIO_H

// Copyright Franck Pascutti 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <utility>
#include <vector>
#include <okra/step.h>

namespace okra {

template<typename Char, typename Traits = std::char_traits<Char> >
class basic_scenario
{
private:
    typedef basic_scenario<Char, Traits> this_type;
    typedef std::basic_string<Char, Traits> string_type;
    typedef basic_step<Char, Traits> step_type;

public:
    basic_scenario(string_type name, std::string file, int line)
        : name_(std::move(name)),
		  file_(std::move(file)),
		  line_(line),
          steps_()
    { }

    void add_step(step_type& s)
    {
        steps_.push_back(s);
    }

    void add_step(const step_type& s)
    {
        steps_.push_back(s);
    }

    void add_step(step_type&& s)
    {
        steps_.push_back(std::move(s));
    }

    template<typename... Args>
    void add_step(Args&&... args)
    {
        steps_.emplace_back(std::forward<Args>(args)...);
    }

    const string_type& name() const OKRA_NOEXCEPT
    {
        return name_;
    }

	const std::string& file() const OKRA_NOEXCEPT
	{
		return file_;
	}

	int line() const OKRA_NOEXCEPT
	{
		return line_;
	}

    template<typename Framework, typename Registry, typename... Args>
    void operator()(Framework& fw, const Registry& reg, Args&&... args) const
    {
        framework_scope<Framework> scope(fw, *this);
        auto end = steps_.end();
        for(auto it = steps_.begin(); it != end; ++it)
        {
            (*it)(fw, reg, std::forward<Args>(args)...);
            if(fw.has_failed())
                break;
        }
    }

private:
    template<typename Framework>
    class framework_scope
    {
    public:
        framework_scope(Framework& fw, const this_type& scenario)
            : fw_(fw),
              scenario_(scenario)
        {
            fw_.before_scenario(scenario_);
        }
        ~framework_scope() OKRA_NOEXCEPT
        {
#if OKRA_COMPILER_CXX_NOEXCEPT
            fw_.after_scenario(scenario_);
#else
            // simulate noexcept destructors when unavailable
            try { fw_.after_scenario(scenario_); } catch(...) { std::terminate(); }
#endif
        }
    private:
        Framework& fw_;
        const this_type& scenario_;
    };

private:
    string_type name_;
	std::string file_;
	int line_;
    std::vector<step_type> steps_;
};

typedef basic_scenario<char> scenario;
typedef basic_scenario<wchar_t> wscenario;

}

#endif // OKRA_SCENARIO_H
