#pragma once

#include "pch.h"

struct HSession
{
    sockaddr_in address;
    SOCKET      socket;

    HSession()
    {
        ZeroMemory(&address, sizeof(address));
        socket = INVALID_SOCKET;
    }

    void Dispatch(DWORD transfer, HOverlap* overlap) {}

    void SendPacket(const char* data, int size) { send(socket, data, size, 0); }

    void RecvPacket() { recv(socket, nullptr, 0, 0); }

    void AsyncRecv()
    {
        HOverlap* overlap = new HOverlap();
        overlap->rwMode   = RW_MODE::RW_READ;
        overlap->rwFlag   = RW_FLAG::RW_RECV;
        DWORD flags       = 0;
        WSARecv(socket, &overlap->wsabuf, 1, nullptr, &flags, overlap, nullptr);
    }
};

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

    void DelUser();

    HSession* GetSession(SOCKET socket);
};
