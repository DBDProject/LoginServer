#include "pch.h"
#include "HCommand.h"
#include "HNetwork.h"

void HCommand::Init()
{
    m_commandMap["help"]      = HCommand::CommandHelp;
    m_commandMap["list"]      = HCommand::CommandUserList;
    m_commandMap["matchlist"] = HCommand::CommandMatchList;
    m_commandMap["maxplayer"] = HCommand::CommandSetMatchPlayer;
    m_commandMap["say"]       = HCommand::CommandSay;
    m_commandMap["exit"]      = HCommand::CommandExit;
    m_commandMap["ovlist"]    = HCommand::CommandOverlapList;
}

void HCommand::ProcessCommand(const std::string& command)
{
    if (command.size() <= 0 || command[0] != '/')
    {
        LOG_INFO("명령어는 '/'로 시작해야 합니다.\n")
        return;
    }

    std::string cmd = std::move(command.substr(1, command.size()));
    std::string token;

    std::istringstream iss(cmd);
    iss >> token;

    if (m_commandMap.contains(token))
        m_commandMap[token](cmd);
    else
        LOG_INFO("존재하지 않는 명령어 입니다. /help 참조\n")
}

void HCommand::CommandHelp(const std::string& command)
{
    LOG_INFO("Command List\n")
    LOG_INFO("/help : Show command list\n")
    LOG_INFO("/list : Show user list\n")
    LOG_INFO("/matchlist : Show match list\n")
    LOG_INFO("/maxplayer <인원 수> : 매칭 최대 인원을 설정한다.\n")
    LOG_INFO("/say <할말> : Send message to all user\n")
    LOG_INFO("/exit : Server shutdown\n")
    LOG_INFO("/ovlist : Show overlap list\n")
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
    if (command.size() <= 4)
    {
        LOG_INFO("메시지를 입력해주세요. /say <msg>\n");
        return;
    }

    std::string msg;
    std::string serverPrefix = "[Server] : ";

    msg = command.substr(4);

    serverPrefix.append(msg);
    HPacketProcessor::SendChatMsg(serverPrefix);
}

void HCommand::CommandMatchList(const std::string& command)
{
    H_NETWORK.m_matching->PrintWaitingList();
}

void HCommand::CommandSetMatchPlayer(const std::string& command)
{
    std::string        token;
    std::istringstream iss(command);
    iss >> token;
    if (iss.eof())
    {
        LOG_INFO("인원 수를 입력해주세요. /maxplayer <인원 수>\n");
        return;
    }
    int player = 0;
    iss >> player;
    if (player < 2)
    {
        LOG_INFO("인원 수는 2이상이여야 합니다.\n");
        return;
    }

    H_NETWORK.m_matching->SetMatchPlayer(player);
    LOG_INFO("매칭 최대 인원이 {}명으로 설정되었습니다.\n", player);
    HPacketProcessor::SendChatMsg("[Server]매칭 최대 인원이 " + std::to_string(player) +
                                  "명으로 설정되었습니다.");
}
