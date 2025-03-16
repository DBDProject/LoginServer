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
    bool           isReady;
    UINT           matchID;
};

struct MatchInfo
{
    std::list<PlayerInfo> survivor;
    PlayerInfo            killer;

    unsigned int matchPlayer;
};

class HMatching
{
private:
    std::map<SOCKET, std::shared_ptr<PlayerInfo>> m_survivorMap;
    std::map<SOCKET, std::shared_ptr<PlayerInfo>> m_killerMap;
    std::map<UINT, std::shared_ptr<MatchInfo>>    m_matchMap;

    UINT m_setMatchPlayer = 3;

private:
    void UpdateMatchQueue();
    void UpdatePlayerReady();

public:
    HMatching()  = default;
    ~HMatching() = default;

    void SetMatchPlayer(const UINT inPlayer);
    UINT GetMatchPlayer() const;

    void AddSurvivorToMatch(const std::shared_ptr<PlayerInfo> player);
    void AddKillerToMatch(const std::shared_ptr<PlayerInfo> player);

    void DeleteSurvivorFromMatch(const SOCKET inSocket);
    void DeleteKillerFromMatch(const SOCKET inSocket);

    void PrintWaitingList();
    void Update();
};
