#include "pch.h"
#include "DBDLoginServer.h"

void DBDLoginServer::Init()
{
    H_NETWORK.Init();
    m_iocp.Init();
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

void DBDLoginServer::Run()
{
    Init();
    while (true)
    {
        this->Update();
    }
    this->Release();
}
