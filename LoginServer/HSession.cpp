#include "pch.h"
#include "HSession.h"

bool HSession::Connect(SOCKET socket, const sockaddr_in& address)
{
    if (IsConnected(socket))
        return false;

    UserSession userSession;
    userSession.address     = address;
    userSession.socket      = socket;
    userSession.isConnected = true;
    m_userSessions[socket]  = userSession;

    LOG_INFO("===============================================")
    LOG_INFO("Client Connected : IP : {} Port : {}",
             inet_ntoa(address.sin_addr),
             ntohs(address.sin_port))
    LOG_INFO("===============================================")

    return true;
}

bool HSession::DisConnect(SOCKET socket)
{
    if (m_userSessions.contains(socket))
    {
        m_userSessions.erase(socket);
        closesocket(socket);
        return true;
    }
    return false;
}

bool HSession::IsConnected(SOCKET socket)
{
    if (!m_userSessions.contains(socket))
        return false;

    return m_userSessions[socket].isConnected;
}
