
// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <gtest/gtest.h>
#include <okra/detail/wstring_convert.h>

namespace {

template<typename Char>
struct custom_char_traits : public std::char_traits<Char>
{ };

}

TEST(wstring_convert, standard_wstring)
{
    auto res = okra::detail::wstring_convert::to_bytes(std::wstring(L"I'm Olaf and I like warm hugs!"));
    ASSERT_STREQ("I'm Olaf and I like warm hugs!", res.c_str()) << "'to_bytes' failed to convert input string.";
}

TEST(wstring_convert, custom_wstring)
{
    auto res = okra::detail::wstring_convert::to_bytes(std::basic_string<wchar_t, custom_char_traits<wchar_t> >(L"I'm Olaf and I like warm hugs!"));
    ASSERT_STREQ("I'm Olaf and I like warm hugs!", res.c_str()) << "'to_bytes' failed to convert input string.";
}
