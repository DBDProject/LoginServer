#include "pch.h"
#include "HIocp.h"
#include "HNetwork.h"

void HIocp::Init()
{
    LOG_INFO("Iocp init\n")
    m_threadPool.Init();
    m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_hIocp == NULL)
        exit(1);

    m_threadPool.SetTask(std::bind(&HIocp::WorkerProcess, this));
    m_threadPool.Run();
    LOG_INFO("Iocp init complete\n")
}

void HIocp::Release()
{
    m_threadPool.Release();
    CloseHandle(m_hIocp);
    LOG_INFO("Iocp release\n")
}

void HIocp::WorkerProcess()
{
    DWORD     dwTransfer;
    ULONG_PTR completionKey;
    HOverlap* lpOverlapped = nullptr;

    bool isPacketProcess = false;
    bool isDisconnected  = false;

    while (HNetwork::m_isRunning)
    {
        BOOL ret = GetQueuedCompletionStatus(m_hIocp,
                                             &dwTransfer,
                                             &completionKey,
                                             reinterpret_cast<LPOVERLAPPED*>(&lpOverlapped),
                                             INFINITE);

        HSession* pSession = (HSession*)completionKey;

        if (!lpOverlapped || !pSession)
            continue;

        if (dwTransfer == 0)
        {
            isDisconnected = true;
        }

        if (ret == TRUE)
        {
            switch (lpOverlapped->rwFlag)
            {
            case RW_FLAG::RW_END:
                isDisconnected = true;
                break;
            case RW_FLAG::RW_RECV:
                ProcessAsyncRecv(pSession, lpOverlapped, dwTransfer);
                break;

            case RW_FLAG::RW_SEND:
                ProcessAsyncSend(pSession, lpOverlapped, dwTransfer);
                break;
            }
        }
        else
        {
            isDisconnected = true;
        }

        if (isDisconnected)
        {
            H_NETWORK.m_sessionManager->DisConnect(pSession->socket);
            H_NETWORK.DeleteOverlap(lpOverlapped);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void HIocp::ProcessAsyncRecv(HSession* pSession, HOverlap* overlap, DWORD transfer)
{
    if (!overlap || !pSession)
        return;

    memcpy(pSession->readPacket, overlap->buffer, transfer);
    overlap->readPos += transfer;

    if (overlap->readPos >= PACKET_HEADER_SIZE)
    {
        HPACKET* packet = (HPACKET*)pSession->readPacket;

        if (packet->ph.len >= overlap->readPos)
        {
            pSession->readPacket[packet->ph.len] = '\0';
            {
                std::lock_guard<std::mutex> lock(m_addPacketMutex);
                H_NETWORK.AddPacket(pSession->socket, packet);
            }

            H_NETWORK.DeleteOverlap(overlap);
            pSession->AsyncRecv();
        }
    }
}

void HIocp::ProcessAsyncSend(HSession* pSession, HOverlap* overlap, DWORD transfer)
{
    if (!overlap || !pSession)
        return;

    memcpy(pSession->sendPacket, overlap->buffer, transfer);
    overlap->writePos += transfer;

    if (overlap->writePos >= PACKET_HEADER_SIZE)
    {
        HPACKET* packet = (HPACKET*)pSession->sendPacket;

        if (packet->ph.len >= overlap->writePos)
        {
            pSession->sendPacket[transfer] = '\0';
            H_NETWORK.DeleteOverlap(overlap);
        }
    }
}
