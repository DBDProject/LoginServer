#pragma once

#include "pch.h"

struct UserSession
{
    sockaddr_in address;
    SOCKET      socket;
    bool        isConnected;
};

class HSession
{
private:
    std::map<SOCKET, UserSession> m_userSessions;

public:
    HSession()  = default;
    ~HSession() = default;

    bool Connect(SOCKET socket, const sockaddr_in& address);
    bool DisConnect(SOCKET socket);
    bool IsConnected(SOCKET socket);
};
