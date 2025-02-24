#include "pch.h"
#include "HCommand.h"
#include "HNetwork.h"

void HCommand::InitCommand()
{
    m_commandMap["help"]   = HCommand::CommandHelp;
    m_commandMap["list"]   = HCommand::CommandUserList;
    m_commandMap["exit"]   = HCommand::CommandExit;
    m_commandMap["ovlist"] = HCommand::CommandOverlapList;
    m_commandMap["say"]    = HCommand::CommandSay;
}

void HCommand::ProcessCommand(const std::string& command)
{
    if (command.size() <= 0 && command[0] != '/')
        return;

    std::string cmd = std::move(command.substr(1, command.size()));
    std::string token;

    std::istringstream iss(cmd);
    iss >> token;

    if (m_commandMap.contains(token))
        m_commandMap[token](cmd);
    else
        LOG_INFO("Unknown command\n")
}

void HCommand::CommandHelp(const std::string& command)
{
    LOG_INFO("Command List\n")
    LOG_INFO("/help : Show command list\n")
    LOG_INFO("/list : Show user list\n")
    LOG_INFO("/ovlist : Show overlap list\n")
    LOG_INFO("/say : Send message to all user\n")
    LOG_INFO("/exit : Server shutdown\n")
}

void HCommand::CommandExit(const std::string& command)
{
    H_NETWORK.StopServer();
}

void HCommand::CommandUserList(const std::string& command)
{
    H_NETWORK.m_sessionManager->PrintUserList();
}

void HCommand::CommandOverlapList(const std::string& command)
{
    H_NETWORK.PrintOverlapList();
}

void HCommand::CommandSay(const std::string& command)
{
    std::string msg;
    msg = std::move(command.substr(4, command.size()));

    HPACKET packet;
    packet.ph.len  = PACKET_HEADER_SIZE + msg.size() * sizeof(char);
    packet.ph.type = PACKET_CHAT_MSG;
    strcpy_s(packet.msg, msg.c_str());
    H_NETWORK.m_sessionManager->Broadcast(reinterpret_cast<char*>(&packet), packet.ph.len);
}
