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
