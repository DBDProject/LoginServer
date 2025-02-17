#pragma once

#include "pch.h"

struct UserSession
{
    sockaddr_in address;
    SOCKET      socket;
};

class HSession
{
private:
    std::map<SOCKET, UserSession> m_userSessions;

public:
    bool AddUser(SOCKET socket, sockaddr_in address);
    bool RemoveUser(SOCKET socket);
    bool IsConnected(SOCKET socket);
};
