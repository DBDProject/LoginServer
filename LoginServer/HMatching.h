#pragma once

#include "pch.h"

enum class ECharacterType : UINT8
{
    NONE = 0,
    MEG,
    DRACULA
};

struct PlayerInfo
{
    SOCKET         socket;
    sockaddr_in    address;
    ECharacterType characterType;
};

struct MatchInfo
{
    std::list<PlayerInfo> survivor;
    bool                  isSurvivorReady;

    PlayerInfo killer;
    bool       isKillerReady;

    const unsigned int matchPlayer;
};

class HMatching
{
private:
    std::map<SOCKET, PlayerInfo> m_survivorMap;
    std::map<SOCKET, PlayerInfo> m_killerMap;
    std::queue<MatchInfo>        m_matchQueue;

    UINT m_setMatchPlayer = 3;

public:
    HMatching()  = default;
    ~HMatching() = default;

    void SetMatchPlayer(const UINT inPlayer);
    UINT GetMatchPlayer() const;

    void PrintWaitingList();
};
