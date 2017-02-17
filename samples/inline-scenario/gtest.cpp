
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <gtest/gtest.h>
#include <okra/googletest/framework.h>
#include <okra/ignore.h>
#include <okra/impl.h>
#include <okra/re.h>
#include <okra/scenario.h>

#ifdef OKRA_SAMPLES_USE_WCHAR_T

typedef wchar_t okra_char_type;
#define OKRA_TEXT(t) L ## t

#else

typedef char okra_char_type;
#define OKRA_TEXT(t) t

#endif

namespace {

unsigned long long factorial(unsigned long long)
{
    return 1ull;
}

okra::googletest::framework framework__;
auto registry__ = okra::impl(okra::re::basic_regex<okra_char_type>(OKRA_TEXT("Given I have the number (\\d+)")), [](okra::ignore, unsigned long long& input, okra::ignore, const std::basic_string<okra_char_type>& arg) { input = std::stoull(arg); })
                | okra::impl(okra::re::basic_regex<okra_char_type>(OKRA_TEXT("When I compute its factorial")), [](okra::ignore, unsigned long long input, unsigned long long& result) { result = factorial(input); })
                | okra::impl(okra::re::basic_regex<okra_char_type>(OKRA_TEXT("Then I see the number (\\d+)")), [](okra::ignore, unsigned long long input, unsigned long long result, const std::basic_string<okra_char_type>& arg) { auto exp = std::stoull(arg); ASSERT_EQ(exp, result) << "Invalid factorial computed for '" << input << "'."; });
}

TEST(factorial, factorial_of_0)
{
    okra::basic_scenario<okra_char_type> s(OKRA_TEXT("Factorial of 0"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Given I have the number 0"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("When I compute its factorial"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Then I see the number 1"), __FILE__, __LINE__);

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

TEST(factorial, factorial_of_1)
{
    okra::basic_scenario<okra_char_type> s(OKRA_TEXT("Factorial of 1"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Given I have the number 1"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("When I compute its factorial"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Then I see the number 1"), __FILE__, __LINE__);

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

TEST(factorial, factorial_of_2)
{
    okra::basic_scenario<okra_char_type> s(OKRA_TEXT("Factorial of 2"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Given I have the number 2"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("When I compute its factorial"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Then I see the number 2"), __FILE__, __LINE__);

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

TEST(factorial, factorial_of_3)
{
    okra::basic_scenario<okra_char_type> s(OKRA_TEXT("Factorial of 3"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Given I have the number 3"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("When I compute its factorial"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Then I see the number 6"), __FILE__, __LINE__);

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

TEST(square, square_of_4)
{
    okra::basic_scenario<okra_char_type> s(OKRA_TEXT("Square of 4"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Given I have the number 4"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("When I compute its square"), __FILE__, __LINE__);
    s.add_step(OKRA_TEXT("Then I see the number 16"), __FILE__, __LINE__);

    unsigned long long input, result;
    s(framework__, registry__, input, result);
}

