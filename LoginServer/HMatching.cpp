#include "pch.h"
#include "HMatching.h"

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

void HMatching::PrintWaitingList()
{
    LOG_INFO("==================================================\n");
    LOG_INFO("Match Player : {}\n\n", m_setMatchPlayer);
    LOG_INFO("Survivor List\n");
    for (const auto& [socket, player] : m_survivorMap)
    {
        LOG_INFO("Socket : {}\n", socket);
    }
    LOG_INFO("\nKiller List\n");
    for (const auto& [socket, player] : m_killerMap)
    {
        LOG_INFO("Socket : {}\n", socket);
    }
    LOG_INFO("==================================================\n");
}

void HMatching::UpdateMatchQueue()
{
    // if (m_survivorMap.size() >= m_setMatchPlayer - 1 && m_killerMap.size() >= 1)
    //{
    //     std::shared_ptr<MatchInfo> match = std::make_shared<MatchInfo>();

    //    match->matchPlayer = m_setMatchPlayer;
    //    for (const auto& [socket, player] : m_survivorMap)
    //    {
    //        match->survivor.push_back(player);
    //    }
    //    if (m_killerMap.size() > 0)
    //    {
    //        match->killer        = m_killerMap.begin()->second;
    //        match->isKillerReady = false;
    //    }
    //    match->isSurvivorReady = false;

    //    m_matchQueue.push(match);
    //}
}

void HMatching::UpdatePlayerReady()
{
    // if (m_matchQueue.empty())
    //     return;
    // auto match = m_matchQueue.front();
    // if (match->isSurvivorReady && match->isKillerReady)
    //{
    //     m_matchQueue.pop();
    //     m_survivorMap.clear();
    //     m_killerMap.clear();
    // }
}

void HMatching::Update()
{
    UpdateMatchQueue();
    UpdatePlayerReady();
}
