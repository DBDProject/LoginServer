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
#include <memory>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")


#ifdef _DEBUG
    #pragma comment(lib, "fmt/lib/fmtd.lib")
#else
    #pragma comment(lib, "fmt/lib/fmt.lib")
#endif

#define FMT_UNICODE 0
#include <fmt/core.h>
#include <fmt/color.h>

#define LOG_ERROR(format, ...) \
    fmt::print(fmt::fg(fmt::color::indian_red), FMT_STRING(format), ##__VA_ARGS__##);
#define LOG_INFO(format, ...) \
    fmt::print(fmt::fg(fmt::color::green_yellow), FMT_STRING(format), ##__VA_ARGS__##);
#define LOG_WARNING(format, ...) \
    fmt::print(fmt::fg(fmt::color::yellow), FMT_STRING(format), ##__VA_ARGS__##);


// #define DEBUG_PRINT
#include "HConvention.h"
#include "HSingleton.h"
#include "HProtocol.h"
#include "HOverlap.h"

using namespace std::chrono;
