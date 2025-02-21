#include "pch.h"
#include "HSessionManager.h"

HSessionManager::~HSessionManager()
{
    for (auto& [socket, userSession] : m_userSessions)
    {
        closesocket(userSession.socket);
    }
}

bool HSessionManager::Connect(SOCKET socket, const sockaddr_in& address)
{
    if (IsConnected(socket))
        return false;

    UserSession userSession;
    userSession.address       = address;
    userSession.socket        = socket;
    userSession.connectedTime = steady_clock::now();

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_userSessions[socket] = userSession;
    }

    LOG_INFO("===============================================\n")
    LOG_INFO("Client Connected : IP : {} Port : {}\n",
             inet_ntoa(address.sin_addr),
             ntohs(address.sin_port))
    LOG_INFO("===============================================\n")

    return true;
}

bool HSessionManager::DisConnect(SOCKET socket)
{
    UserSession userSession;
    bool        isDisConnect = false;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_userSessions.contains(socket))
        {
            userSession = m_userSessions[socket];
            m_userSessions.erase(socket);
            isDisConnect = true;
        }
    }

    if (isDisConnect)
    {
        closesocket(userSession.socket);
        LOG_INFO("===============================================\n")
        LOG_INFO("Client disconnected : IP : {} Port : {}\n",
                 inet_ntoa(userSession.address.sin_addr),
                 ntohs(userSession.address.sin_port))
        LOG_INFO("===============================================\n")
    }

    return isDisConnect;
}

bool HSessionManager::IsConnected(SOCKET socket)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_userSessions.contains(socket);
}

void HSessionManager::Broadcast(const char* data, int size) {}

void HSessionManager::CheckTimeOut()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& [socket, userSession] : m_userSessions)
    {
        auto currentTime = steady_clock::now();
        auto duration    = duration_cast<seconds>(currentTime - userSession.connectedTime);

        if (duration.count() > TIME_OUT)
            userSession.SendPacket("Time out test", 8);
    }
}

UserSession* HSessionManager::GetUserSession(SOCKET socket)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_userSessions.contains(socket))
        return nullptr;

    return &m_userSessions[socket];
}
