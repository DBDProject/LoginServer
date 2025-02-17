#pragma once

#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <thread>
#include <algorithm>
#include <functional>
#include <condition_variable>
#include <chrono>
#include <memory>
#include <fmt/core.h>
#include <fmt/color.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")


#ifdef _DEBUG
    #pragma comment(lib, "fmt/lib/fmtd.lib")
#else
    #pragma comment(lib, "fmt/lib/fmt.lib")
#endif

#include "HConvention.h"
#include "HSingleton.h"

using namespace std::chrono;

#define LOG_ERROR(format, ...) \
    fmt::print(fmt::fg(fmt::color::dark_red), FMT_STRING(format), ##__VA_ARGS__)
#define LOG_INFO(format, ...) \
    fmt::print(fmt::fg(fmt::color::green_yellow), FMT_STRING(format), ##__VA_ARGS__)
#define LOG_WARNING(format, ...) \
    fmt::print(fmt::fg(fmt::color::yellow), FMT_STRING(format), ##__VA_ARGS__)
