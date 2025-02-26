#include "pch.h"
#include "HIocp.h"
#include "HNetwork.h"

/**
 * @brief Initializes the IOCP handler
 *
 * Creates an IOCP handle, initializes the thread pool and sets up worker threads
 * to process I/O completion notifications.
 */
void HIocp::Init()
{
    LOG_INFO("Iocp init\n");
    m_threadPool.Init();
    m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_hIocp == NULL)
        exit(1);

    m_threadPool.SetTask(std::bind(&HIocp::WorkerProcess, this));
    m_threadPool.Run();
    LOG_INFO("Iocp init complete\n");
}

/**
 * @brief Releases resources used by the IOCP handler
 *
 * Performs cleanup by releasing the thread pool and closing the IOCP handle.
 */
void HIocp::Release()
{
    m_threadPool.Release();
    CloseHandle(m_hIocp);
    LOG_INFO("Iocp release\n");
}

/**
 * @brief Worker thread process for handling I/O completion notifications
 *
 * Continuously processes completion notifications from the IOCP queue and handles
 * them based on their completion type (receive, send, or disconnection).
 */
void HIocp::WorkerProcess()
{
    DWORD     dwTransfer;
    ULONG_PTR completionKey;
    HOverlap* lpOverlapped = nullptr;

    bool isDisconnected = false;

    while (HNetwork::m_isRunning)
    {
        BOOL ret = GetQueuedCompletionStatus(m_hIocp,
                                             &dwTransfer,
                                             &completionKey,
                                             reinterpret_cast<LPOVERLAPPED*>(&lpOverlapped),
                                             INFINITE);

        HSession* pSession = (HSession*)completionKey;
        isDisconnected     = false;

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
            case RW_FLAG::END:
                isDisconnected = true;
                break;
            case RW_FLAG::RECV:
                ProcessAsyncRecv(pSession, lpOverlapped, dwTransfer);
                break;

            case RW_FLAG::SEND:
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
            LOG_DEBUG("플레이어 종료로 인한 오버랩 삭제\n")
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

/**
 * @brief Processes asynchronous receive completion
 *
 * @param pSession The session that received data
 * @param overlap The overlapped structure containing buffer information
 * @param transfer The number of bytes transferred in this operation
 *
 * Handles the received data by copying it to the session's read buffer and
 * processing complete packets when enough data is available.
 */
void HIocp::ProcessAsyncRecv(HSession* pSession, HOverlap* overlap, DWORD transfer)
{
    if (!overlap || !pSession)
        return;

    if (!overlap->buffer)
        return;

    memcpy(&pSession->recvPacket[overlap->readPos.load()], overlap->buffer, transfer);
    overlap->readPos += transfer;

    if (overlap->readPos >= PACKET_HEADER_SIZE)
    {
        HPACKET* packet = reinterpret_cast<HPACKET*>(pSession->recvPacket);

        if (packet->ph.len <= overlap->readPos)
        {
            {
                std::lock_guard<std::mutex> lock(m_addPacketMutex);
                H_NETWORK.AddPacket(pSession->socket, reinterpret_cast<HPACKET*>(overlap->GetBuffer()));
                // 만약 예외가 발생하면 메모리 누수가 날 수 있음
                // 언젠가 RAII로 수정해야함
            }

            H_NETWORK.DeleteOverlap(overlap);
            pSession->AsyncRecv();
            LOG_DEBUG("Recv 오버랩 삭제\n")
        }
    }
}

/**
 * @brief Processes asynchronous send completion
 *
 * @param pSession The session that sent data
 * @param overlap The overlapped structure containing buffer information
 * @param transfer The number of bytes transferred in this operation
 *
 * Handles the completion of a send operation by updating buffers and cleaning up
 * when the operation is complete.
 */
void HIocp::ProcessAsyncSend(HSession* pSession, HOverlap* overlap, DWORD transfer)
{
    if (!overlap || !pSession)
        return;

    overlap->writePos += transfer;

    if (overlap->writePos >= PACKET_HEADER_SIZE)
    {
        if (static_cast<int>(overlap->InternalHigh) <= overlap->writePos)
        {
            H_NETWORK.DeleteOverlap(overlap);
            LOG_DEBUG("Send 오버랩 삭제\n")
        }
    }
}
