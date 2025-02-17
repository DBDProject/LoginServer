#include "pch.h"
#include "HSession.h"

bool HSession::AddUser(SOCKET socket, sockaddr_in address)
{
    if (m_userSessions.contains(socket))
    {
        if (IsConnected(socket))
        {
            return false;
        }

        m_userSessions[socket].address = address;
    }
    else
    {
        UserSession userSession;
        userSession.address    = address;
        userSession.socket     = socket;
        m_userSessions[socket] = userSession;
    }
}

bool HSession::RemoveUser(SOCKET socket)
{
    if (m_userSessions.contains(socket))
    {
        m_userSessions.erase(socket);
        return true;
    }
    return false;
}

bool HSession::IsConnected(SOCKET socket)
{

    return false;
}
