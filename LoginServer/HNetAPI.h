#pragma once

#include <Windows.h>
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

    // CP949 → UTF-8 변환 함수
    static std::string ConvertCP949ToUTF8(const std::string& cp949String)
    {
        // 1. CP949 → UTF-16 변환 (WideChar)
        int wideCharSize = MultiByteToWideChar(949, 0, cp949String.c_str(), -1, nullptr, 0);
        if (wideCharSize <= 0)
            return "";

        std::wstring wideString(wideCharSize, 0);
        MultiByteToWideChar(949, 0, cp949String.c_str(), -1, &wideString[0], wideCharSize);

        // 2. UTF-16 → UTF-8 변환
        int utf8Size =
        WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (utf8Size <= 0)
            return "";

        std::string utf8String(utf8Size, 0);
        WideCharToMultiByte(CP_UTF8,
                            0,
                            wideString.c_str(),
                            -1,
                            &utf8String[0],
                            utf8Size,
                            nullptr,
                            nullptr);

        return std::move(utf8String);
    }

    // UTF-8 → CP949 변환 함수
    static std::string ConvertUTF8ToCP949(const std::string& utf8_str)
    {
        // UTF-8 → UTF-16 변환
        int utf16_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
        if (utf16_size == 0)
            return "";

        std::wstring utf16_str(utf16_size, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &utf16_str[0], utf16_size);

        // UTF-16 → CP949 변환
        int cp949_size =
        WideCharToMultiByte(CP_ACP, 0, utf16_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (cp949_size == 0)
            return "";

        std::string cp949_str(cp949_size, 0);
        WideCharToMultiByte(CP_ACP,
                            0,
                            utf16_str.c_str(),
                            -1,
                            &cp949_str[0],
                            cp949_size,
                            nullptr,
                            nullptr);

        return std::move(cp949_str);
    }
};
