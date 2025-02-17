#include "pch.h"
#include "DBDLoginServer.h"

void DBDLoginServer::Init()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    H_NETWORK.Init();
}

void DBDLoginServer::Update()
{
    m_systemTimer.Update();
}

void DBDLoginServer::Release()
{
    H_NETWORK.Release();
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
