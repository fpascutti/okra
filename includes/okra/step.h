#ifndef OKRA_STEP_H
#define OKRA_STEP_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <utility>
#include <okra/config.h>

namespace okra {

template<typename Char, typename Traits = std::char_traits<Char> >
class basic_step
{
private:
    typedef basic_step<Char, Traits> this_type;
    typedef std::basic_string<Char, Traits> string_type;

public:
    basic_step(string_type text)
        : text_(std::move(text))
    { }

    const string_type& text() const OKRA_NOEXCEPT
    {
        return text_;
    }

    template<typename Framework, typename Registry, typename... Args>
    void operator()(Framework& fw, const Registry& reg, Args&&... args) const
    {
        framework_scope<Framework> scope(fw, *this);
        if(!reg(text(), fw, std::forward<Args>(args)...))
            fw.undefined_step(*this);
    }

private:
    template<typename Framework>
    class framework_scope
    {
    public:
        framework_scope(Framework& fw, const this_type& step)
            : fw_(fw),
              step_(step)
        {
            fw_.before_step(step_);
        }
        ~framework_scope() OKRA_NOEXCEPT
        {
#if OKRA_COMPILER_CXX_NOEXCEPT
            fw_.after_step(step_);
#else
            // simulate noexcept destructors when unavailable
            try { fw_.after_step(step_); } catch(...) { std::terminate(); }
#endif
        }
    private:
        Framework& fw_;
        const this_type& step_;
    };

private:
    string_type text_;
};

typedef basic_step<char> step;
typedef basic_step<wchar_t> wstep;

}

#endif // OKRA_STEP_H
