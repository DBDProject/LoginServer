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
    m_isRunning = false;
    m_threadPool.Release();
    CloseHandle(m_hIocp);
    LOG_INFO("Iocp release\n")
}

void HIocp::WorkerProcess()
{
    DWORD     dwTransfer;
    ULONG_PTR completionKey;
    HOverlap* lpOverlapped = nullptr;

    bool isClientDisconnect = false;

    while (m_isRunning)
    {
        BOOL ret = GetQueuedCompletionStatus(m_hIocp,
                                             &dwTransfer,
                                             &completionKey,
                                             (LPOVERLAPPED*)&lpOverlapped,
                                             INFINITE);

        HSession* pSession = (HSession*)completionKey;
        isClientDisconnect = false;

        if (!lpOverlapped || !pSession)
            continue;

        if (ret == TRUE)
        {
            if (lpOverlapped->rwFlag == RW_FLAG::RW_END || dwTransfer == 0)
                isClientDisconnect = true;
            else
                pSession->Dispatch(dwTransfer, lpOverlapped);
        }
        else
        {
            isClientDisconnect = true;
        }

        if (isClientDisconnect)
            H_NETWORK.m_sessionManager->DisConnect(pSession->socket);


        if (lpOverlapped)
        {
            delete lpOverlapped;
            lpOverlapped = nullptr;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
