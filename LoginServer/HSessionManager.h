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

    void Broadcast(const char* data, int size);
    void Broadcast(const char* data, int size, SOCKET socket);

    void DelUser();

    HSession* GetSession(SOCKET socket);
};
