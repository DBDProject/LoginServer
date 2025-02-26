#include "pch.h"
#include "DBDLoginServer.h"

void DBDLoginServer::Init()
{
    HCommand::InitCommand();
    H_NETWORK.Init();
    H_NETWORK.CreateServer(m_serverPort);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::thread workerThread(&DBDLoginServer::InputProcess, this);
    workerThread.detach();
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

void DBDLoginServer::InputProcess()
{
    LOG_INFO("명령어 정보 /help\n")
    std::string input;
    input.assign(256, '\0');

    while (HNetwork::m_isRunning)
    {
        std::getline(std::cin, input);
        HCommand::ProcessCommand(input);
    }
    LOG_INFO("Input process terminated\n")
}

void DBDLoginServer::Run()
{
    Init();

    while (HNetwork::m_isRunning)
    {
        Update();

        if (!H_NETWORK.AcceptClient())
            break;

        H_NETWORK.ProcessPacket();
    }

    Release();
    LOG_INFO("Server terminated\n")
    system("pause");
}
