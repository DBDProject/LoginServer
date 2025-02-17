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

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include "HConvention.h"
#include "HSingleton.h"
using namespace std::chrono;
