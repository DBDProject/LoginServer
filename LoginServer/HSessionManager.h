#pragma once

#include "pch.h"
#include "HSession.h"

class HSessionManager
{
private:
    std::map<SOCKET, HSession> m_hSessions;
    std::queue<SOCKET>         m_disConnectQueue;

    std::mutex m_mutex;

public:
    HSessionManager() = default;
    ~HSessionManager();

    bool IsConnected(SOCKET socket);
    void Connect(SOCKET socket, const sockaddr_in& address);
    void DisConnect(SOCKET socket);

    void Broadcast(const HPACKET* inPacket);

    void      DelUser();
    void      PrintUserList() const;
    HSession* GetSession(SOCKET socket);
};
