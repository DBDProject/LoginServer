#pragma once

#include <string>
#include <string_view>
#include <atlconv.h>

class HNetAPI
{
public:
    static std::wstring ToUnicode(const std::string_view src)
    {
        USES_CONVERSION;
        return std::move(std::wstring(A2W(src.data())));
    }

    static std::string ToMultiByte(const std::wstring_view src)
    {
        USES_CONVERSION;
        return std::move(std::string(W2A(src.data())));
    }
};
