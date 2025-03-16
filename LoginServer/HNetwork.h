#pragma once

#include "pch.h"
#include "HSessionManager.h"
#include "HOverlap.h"
#include "HIocp.h"
#include "HPacketProcessor.h"
#include "HMatching.h"

#define H_NETWORK HNetwork::GetInstance()

class HNetwork : public HSingleton<HNetwork>, HConvention
{
private:
    WSADATA m_wsadata;
    SOCKET  m_serverSocket;
    HIocp   m_iocp;

    std::queue<std::pair<SOCKET, std::shared_ptr<HPACKET>>> m_packetQueue;

    std::set<HOverlap*> m_overlapSet;

    std::mutex m_overlapMutex;
    std::mutex m_addPacketMutex;

    H_SINGLETON_DECLARE(HNetwork)

public:
    std::unique_ptr<HSessionManager> m_sessionManager;
    std::unique_ptr<HMatching>       m_matching;

    inline static bool m_isRunning = true;

private:
    void InitWinSock();
    void ProcessPacket();
    bool AcceptClient();

public:
    void Init() override;
    void Release() override;
    void Update();

    void PrintSockError(int errorCode);
    bool HasSockError(int errorCode);

    void CreateServer(int port);
    void StopServer();

    void AddPacket(SOCKET socket, std::shared_ptr<HPACKET> packet);

    HOverlap* AddOverlap();
    bool      DeleteOverlap(HOverlap* overlap);
    void      PrintOverlapList();

    std::string GetInternalServerIP();
    std::string GetExternalServerIP();
};
