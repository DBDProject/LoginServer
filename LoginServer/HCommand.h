#pragma once

#include "pch.h"

class HCommand
{
private:
    inline static std::map<std::string, std::function<void(const std::string& command)>> m_commandMap;

public:
    static void InitCommand();
    static void ProcessCommand(const std::string& command);
    static void CommandHelp(const std::string& command);
    static void CommandExit(const std::string& command);
    static void CommandUserList(const std::string& command);
    static void CommandOverlapList(const std::string& command);
    static void CommandSay(const std::string& command);
};
