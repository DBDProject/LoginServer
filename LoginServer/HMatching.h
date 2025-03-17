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
    std::string    address;
    ECharacterType characterType;
    bool           isReady;
    UINT           matchID;
};

struct MatchInfo
{
    std::list<std::shared_ptr<PlayerInfo>> survivor;
    std::shared_ptr<PlayerInfo>            killer;

    UINT matchPlayer;
    UINT matchID;
};

class HMatching
{
private:
    std::map<SOCKET, std::shared_ptr<PlayerInfo>> m_survivorMap;
    std::map<SOCKET, std::shared_ptr<PlayerInfo>> m_killerMap;
    std::map<SOCKET, std::shared_ptr<PlayerInfo>> m_watingPlayer;
    std::map<UINT, std::shared_ptr<MatchInfo>>    m_matchMap;

    UINT m_setMatchPlayer = 2;

    inline static UINT m_matchID = 0;

private:
    void UpdateMatchQueue();
    void UpdatePlayerReady(const SOCKET socket);

public:
    HMatching()  = default;
    ~HMatching() = default;

    void SetMatchPlayer(const UINT inPlayer);
    UINT GetMatchPlayer() const;

    void AddSurvivorToMatch(const std::shared_ptr<PlayerInfo> player);
    void AddKillerToMatch(const std::shared_ptr<PlayerInfo> player);

    void DeleteSurvivorFromMatch(const SOCKET inSocket);
    void DeleteKillerFromMatch(const SOCKET inSocket);
    void DeleteWaitingPlayer(const SOCKET inSocket);

    void ReadyPlayer(const SOCKET socket);
    void DeleteMatch(const UINT matchID);

    void PrintWaitingList();
    void Update();
};
