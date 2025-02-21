#pragma once

#include "pch.h"

#define TIME_OUT 3000

struct UserSession
{
    sockaddr_in address;
    SOCKET      socket;

    time_point<steady_clock> connectedTime;

    UserSession()
    {
        ZeroMemory(&address, sizeof(address));
        socket = INVALID_SOCKET;
    }

    void Dispatch(DWORD transfer, HOverlap* overlap) {}

    void SendPacket(const char* data, int size) { send(socket, data, size, 0); }

    void RecvPacket() { recv(socket, nullptr, 0, 0); }
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

    void CheckTimeOut();

    UserSession* GetUserSession(SOCKET socket);
};
