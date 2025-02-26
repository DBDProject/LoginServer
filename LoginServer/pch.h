#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <thread>
#include <algorithm>
#include <functional>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <regex>
#include <memory>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")


#ifdef _DEBUG
    #pragma comment(lib, "fmt/lib/fmtd.lib")
    #pragma comment(lib, "protobuf/lib/libprotobufd.lib")

#else
    #pragma comment(lib, "fmt/lib/fmt.lib")
    #pragma comment(lib, "protobuf/lib/libprotobuf.lib")
#endif

#define DEBUG_PRINT
#define FMT_UNICODE 0
#include <fmt/core.h>
#include <fmt/color.h>

#define LOG_ERROR(format, ...) \
    fmt::print(fmt::fg(fmt::color::indian_red), FMT_STRING(format), ##__VA_ARGS__##);
#define LOG_INFO(format, ...) \
    fmt::print(fmt::fg(fmt::color::green_yellow), FMT_STRING(format), ##__VA_ARGS__##);
#define LOG_WARNING(format, ...) \
    fmt::print(fmt::fg(fmt::color::yellow), FMT_STRING(format), ##__VA_ARGS__##);
#ifdef DEBUG_PRINT
    #define LOG_DEBUG(format, ...) \
        fmt::print(fmt::fg(fmt::color::light_slate_gray), FMT_STRING(format), ##__VA_ARGS__##);
#else
    #define LOG_DEBUG(format, ...)
#endif

#include "HConvention.h"
#include "HSingleton.h"
#include "HProtocol.h"

using namespace std::chrono;

#pragma hdrstop
#include "Packet.pb.h"
