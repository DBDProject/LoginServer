#include "pch.h"
#include "HMatching.h"
#include "HPacketProcessor.h"

void HMatching::SetMatchPlayer(const UINT inPlayer)
{
    m_setMatchPlayer = inPlayer;
}

UINT HMatching::GetMatchPlayer() const
{
    return m_setMatchPlayer;
}

void HMatching::AddSurvivorToMatch(const std::shared_ptr<PlayerInfo> player)
{
    m_survivorMap[player->socket] = player;
}

void HMatching::AddKillerToMatch(const std::shared_ptr<PlayerInfo> player)
{
    m_killerMap[player->socket] = player;
}

void HMatching::DeleteSurvivorFromMatch(const SOCKET inSocket)
{
    m_survivorMap.erase(inSocket);
}

void HMatching::DeleteKillerFromMatch(const SOCKET inSocket)
{
    m_killerMap.erase(inSocket);
}

void HMatching::ReadyPlayer(const SOCKET socket)
{
    if (m_watingPlayer.contains(socket))
    {
        m_watingPlayer[socket]->isReady = true;
        CheckMatchReady(m_watingPlayer[socket]->matchID);
    }
}

void HMatching::DeleteMatch(const UINT matchID)
{
    auto match = m_matchMap[matchID];

    for (const auto& player : match->survivor)
    {
        m_watingPlayer.erase(player->socket);
    }

    m_watingPlayer.erase(match->killer->socket);
    m_matchMap.erase(matchID);
}

bool HMatching::IsMatchingPlayer(const SOCKET socket)
{
    if (m_watingPlayer.contains(socket))
        return true;

    return false;
}

UINT HMatching::GetMatchIDInPlayer(const SOCKET socket)
{
    if (m_watingPlayer.contains(socket))
        return m_watingPlayer[socket]->matchID;
    else
        return 0;
}

std::shared_ptr<MatchInfo> HMatching::GetMatchInfo(const UINT matchID)
{
    if (m_matchMap.contains(matchID))
        return m_matchMap[matchID];
    else
        return nullptr;
}

void HMatching::PrintWaitingList()
{
    LOG_INFO("===============================================\n");
    LOG_INFO("현재 매칭 가능한 인원 수 : {}\n\n", m_setMatchPlayer);
    LOG_INFO("Survivor List\n");
    for (const auto& [socket, player] : m_survivorMap)
    {
        LOG_INFO("- [{}][Type : {}]\n", socket, (int)player->characterType);
    }
    LOG_INFO("\nKiller List\n");
    for (const auto& [socket, player] : m_killerMap)
    {
        LOG_INFO("- [{}][Type : {}]\n", socket, (int)player->characterType);
    }
    LOG_INFO("===============================================\n");
    LOG_INFO("Match List\n");

    for (const auto& [matchID, match] : m_matchMap)
    {
        LOG_INFO("Match ID : {}\n", matchID);
        LOG_INFO("Match Player : {}\n", match->matchPlayer);
        LOG_INFO("Survivor List\n");
        for (const auto& player : match->survivor)
        {
            LOG_INFO("- [{}][Type : {}]\n", player->socket, (int)player->characterType);
        }
        LOG_INFO("\nKiller\n");
        LOG_INFO("- [{}][Type : {}]\n", match->killer->socket, (int)match->killer->characterType);
        LOG_INFO("===============================================\n");
    }
}

void HMatching::UpdateMatchQueue()
{
    if (m_survivorMap.size() >= m_setMatchPlayer - 1 && m_killerMap.size() >= 1)
    {
        std::shared_ptr<MatchInfo> match = std::make_shared<MatchInfo>();

        UINT playerCount = 0;

        match->matchPlayer = m_setMatchPlayer;
        match->matchID     = m_matchID;

        for (const auto& [socket, player] : m_survivorMap)
        {
            if (playerCount >= m_setMatchPlayer - 1)  // 1명 킬러
                break;

            player->matchID = m_matchID;
            match->survivor.push_back(player);
            playerCount++;
        }

        match->killer          = m_killerMap.begin()->second;
        match->killer->matchID = m_matchID;

        m_matchMap[m_matchID] = match;
        m_matchID++;

        for (const auto& playerInfo : match->survivor)
        {
            m_watingPlayer[playerInfo->socket] = playerInfo;
            m_survivorMap.erase(playerInfo->socket);
        }

        m_watingPlayer[match->killer->socket] = match->killer;
        m_killerMap.erase(match->killer->socket);

        HPacketProcessor::SendMatchReady(*match);
    }
}

void HMatching::CheckMatchReady(UINT matchID)
{
    auto match   = m_matchMap[matchID];
    bool isReady = true;
    for (const auto& player : match->survivor)
    {
        if (!player->isReady)
        {
            isReady = false;
            break;
        }
    }

    if (!match->killer->isReady)
        isReady = false;

    if (isReady)
    {
        HPacketProcessor::SendMatchStart(*match);
        DeleteMatch(matchID);
    }
}

void HMatching::Update()
{
    UpdateMatchQueue();
}
