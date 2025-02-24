#pragma once

#include "pch.h"

class HCommand
{
private:
    inline static std::map<std::string, std::function<void()>> m_commandMap;

public:
    static void InitCommand();
    static void ProcessCommand(const std::string& command);
    static void CommandHelp();
    static void CommandExit();
    static void CommandList();
};
