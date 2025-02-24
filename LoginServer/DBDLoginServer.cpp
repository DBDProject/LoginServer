#include "pch.h"
#include "DBDLoginServer.h"

void DBDLoginServer::Init()
{
    H_NETWORK.Init();
    H_NETWORK.CreateServer(m_serverPort);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void DBDLoginServer::Update()
{
    m_systemTimer.Update();
    H_NETWORK.m_sessionManager->DelUser();
}

void DBDLoginServer::Release()
{
    H_NETWORK.Release();
}

void DBDLoginServer::SetServerPort(int port)
{
    m_serverPort = port;
}

void DBDLoginServer::Run()
{
    Init();

    while (true)
    {
        Update();

        if (!H_NETWORK.AcceptClient())
            break;

        H_NETWORK.ProcessPactket();
    }

    Release();
    LOG_INFO("Server terminated\n")
}
