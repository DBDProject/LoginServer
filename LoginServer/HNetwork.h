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

    std::queue<HPACKET*> m_packetQueue;

    H_SINGLETON_DECLARE(HNetwork)

public:
    std::unique_ptr<HSessionManager> m_sessionManager;

private:
    void InitWinSock();

public:
    void Init() override;
    void Release() override;

    bool HasSockError(int errorCode);
    bool AcceptClient();

    void PrintSockError(int errorCode);
    void CreateServer(int port);
    void AddPacket(HPACKET* packet);
    void ProcessPactket();

    std::string GetServerIP();
};
