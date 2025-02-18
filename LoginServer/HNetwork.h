#pragma once

#include "pch.h"
#include "HSingleton.h"
#include "HSession.h"

#define H_NETWORK HNetwork::GetInstance()

class HNetwork : public HSingleton<HNetwork>, HConvention
{
private:
    WSADATA m_wsadata;
    SOCKET  m_serverSocket;

    H_SINGLETON_DECLARE(HNetwork)

public:
    std::shared_ptr<HSession> m_sessionManager;

private:
    void InitWinSock();

public:
    void Init() override;
    void Release() override;

    bool HasSockError();
    bool AcceptClient();

    void PrintSockError();
    void CreateServer(int port);

    std::string GetServerIP();
};
