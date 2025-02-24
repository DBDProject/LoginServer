#include "pch.h"
#include "HSessionManager.h"

HSessionManager::~HSessionManager()
{
    for (auto& [socket, userSession] : m_hSessions)
    {
        closesocket(userSession.socket);
    }
}

void HSessionManager::Connect(SOCKET socket, const sockaddr_in& address)
{
    HSession userSession;
    userSession.address = address;
    userSession.socket  = socket;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_hSessions[socket] = userSession;
    }

    LOG_INFO("===============================================\n")
    LOG_INFO("Client Connected : IP : {} Port : {}\n",
             inet_ntoa(address.sin_addr),
             ntohs(address.sin_port))
    LOG_INFO("===============================================\n")
}

void HSessionManager::DisConnect(SOCKET socket)
{
    bool isDisconnect = false;

    HSession HSession;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_hSessions.contains(socket))
        {
            HSession = m_hSessions[socket];
            m_disConnectQueue.push(socket);
            isDisconnect = true;
        }
    }

    if (isDisconnect)
    {
        LOG_INFO("===============================================\n")
        LOG_INFO("Client disconnected : IP : {} Port : {}\n",
                 inet_ntoa(HSession.address.sin_addr),
                 ntohs(HSession.address.sin_port))
        LOG_INFO("===============================================\n")
    }
}

bool HSessionManager::IsConnected(SOCKET socket)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_hSessions.contains(socket);
}

void HSessionManager::Broadcast(const char* data, int size) {}

void HSessionManager::DelUser()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    while (!m_disConnectQueue.empty())
    {
        SOCKET socket = m_disConnectQueue.front();
        m_hSessions.erase(socket);
        closesocket(socket);
        m_disConnectQueue.pop();
    }
}

HSession* HSessionManager::GetSession(SOCKET socket)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_hSessions.contains(socket))
        return nullptr;

    return &m_hSessions[socket];
}
