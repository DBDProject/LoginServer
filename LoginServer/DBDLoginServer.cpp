#include "pch.h"
#include "DBDLoginServer.h"

void DBDLoginServer::Init()
{
    H_NETWORK.Init();
    m_iocp.Init();
    H_NETWORK.CreateServer(m_serverDesc.ip, m_serverDesc.port);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void DBDLoginServer::Update()
{
    m_systemTimer.Update();
}

void DBDLoginServer::Release()
{
    H_NETWORK.Release();
    m_iocp.Release();
}

void DBDLoginServer::SetServerDesc(const ServerDesc& serverDesc)
{
    m_serverDesc = serverDesc;
}

void DBDLoginServer::Run()
{
    Init();

    LOG_INFO("Server Start\n");

    while (true)
    {
        this->Update();

        if (!H_NETWORK.AcceptClient())
            break;
    }
    this->Release();
}
