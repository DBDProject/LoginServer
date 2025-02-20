#pragma once

#include "pch.h"

struct UserSession
{
    sockaddr_in address;
    SOCKET      socket;

    UserSession()
    {
        ZeroMemory(&address, sizeof(address));
        socket = INVALID_SOCKET;
    }

    void Dispatch(DWORD transfer, HOverlap* overlap) {}
};

class HSessionManager
{
private:
    std::map<SOCKET, UserSession> m_userSessions;

    std::mutex m_mutex;

public:
    HSessionManager() = default;
    ~HSessionManager();

    bool Connect(SOCKET socket, const sockaddr_in& address);
    bool DisConnect(SOCKET socket);
    bool IsConnected(SOCKET socket);

    void Broadcast(const char* data, int size);

    UserSession* GetUserSession(SOCKET socket);
};
