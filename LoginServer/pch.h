#pragma once

#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <memory>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std::chrono;
