#pragma once

#include "pch.h"
#include "HSingleton.h"
#include "HSessionManager.h"
#include "HIocp.h"

#define H_NETWORK HNetwork::GetInstance()

class HNetwork : public HSingleton<HNetwork>, HConvention
{
private:
    WSADATA m_wsadata;
    SOCKET  m_serverSocket;
    HIocp   m_iocp;

    std::queue<std::pair<SOCKET, HPACKET*>> m_packetQueue;
    std::set<HOverlap*>                     m_overlapSet;

    std::mutex m_overlapMutex;

    H_SINGLETON_DECLARE(HNetwork)

public:
    std::unique_ptr<HSessionManager> m_sessionManager;

private:
    void InitWinSock();

public:
    void Init() override;
    void Release() override;

    void PrintSockError(int errorCode);
    bool HasSockError(int errorCode);

    bool AcceptClient();
    void CreateServer(int port);

    void AddPacket(SOCKET socket, HPACKET* packet);
    void ProcessPactket();

    HOverlap* AddOverlap();
    bool      DeleteOverlap(HOverlap* overlap);

    std::string GetServerIP();
};
