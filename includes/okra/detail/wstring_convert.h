#ifndef OKRA_DETAIL_WSTRING_CONVERT_H
#define OKRA_DETAIL_WSTRING_CONVERT_H

// Copyright Franck Pascutti 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if OKRA_CONFIG_USEBOOSTWSTRINGCONVERT

#include <string>
#include <boost/locale/encoding.hpp>

namespace okra { namespace detail {

class wstring_convert
{
public:
    template<typename Traits>
    static std::string to_bytes(const std::basic_string<wchar_t, Traits>& txt)
    {
        return boost::locale::conv::utf_to_utf<char>(txt.c_str(), txt.c_str() + txt.size());
    }
    static std::string to_bytes(const std::wstring& txt)
    {
        return boost::locale::conv::utf_to_utf<char>(txt, boost::locale::conv::stop);
    }
};

} }

#else

#include <cwchar>
#include <locale>
#include <string>
#include <utility>
#include <okra/config.h>

namespace okra { namespace detail {

class wstring_convert
{
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

public:
    template<typename Traits>
    static std::string to_bytes(const std::basic_string<wchar_t, Traits>& txt)
    {
        std::wstring_convert<codecvt> converter("<failed conversion>", L"<failed conversion>");
        return converter.to_bytes(txt.c_str());
    }
    static std::string to_bytes(const std::wstring& txt)
    {
        std::wstring_convert<codecvt> converter("<failed conversion>", L"<failed conversion>");
        return converter.to_bytes(txt);
    }
};

} }

#endif // OKRA_CONFIG_USEBOOSTWSTRINGCONVERT

#endif // OKRA_DETAIL_WSTRING_CONVERT_H
