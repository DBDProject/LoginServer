#include "pch.h"
#include "HCommand.h"
#include "HNetwork.h"

void HCommand::InitCommand()
{
    m_commandMap["help"] = HCommand::CommandHelp;
    m_commandMap["exit"] = HCommand::CommandExit;
    m_commandMap["list"] = HCommand::CommandList;
}

void HCommand::ProcessCommand(const std::string& command)
{
    if (command.size() <= 0 && command[0] != '/')
        return;

    std::string cmd = std::move(command.substr(1, command.size()));

    if (m_commandMap.contains(cmd))
        m_commandMap[cmd]();
    else
        LOG_INFO("Unknown command\n")
}

void HCommand::CommandHelp()
{
    LOG_INFO("Command List\n")
    LOG_INFO("/help : Show command list\n")
    LOG_INFO("/exit : Server shutdown\n")
    LOG_INFO("/list : Show user list\n")
}

void HCommand::CommandExit()
{
    H_NETWORK.StopServer();
}

void HCommand::CommandList()
{
    H_NETWORK.m_sessionManager->PrintUserList();
}
