#include "pch.h"
#include "HIocp.h"

void HIocp::Init()
{
    LOG_INFO("Iocp init\n")
    m_threadPool.Init();
    m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_hIocp == NULL)
        H_NETWORK.PrintSockError();

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

    while (m_isRunning)
    {
        BOOL ret = GetQueuedCompletionStatus(m_hIocp,
                                             &dwTransfer,
                                             &completionKey,
                                             (LPOVERLAPPED*)&lpOverlapped,
                                             INFINITE);

        UserSession* pSession = (UserSession*)completionKey;

        if (!lpOverlapped || !pSession)
            continue;

        if (ret == TRUE)
        {
            if (lpOverlapped->rwFlag == RW_FLAG::RW_END)
            {
                H_NETWORK.m_sessionManager->DisConnect(pSession->socket);
                continue;
            }

            if (dwTransfer == 0)
            {
                lpOverlapped->rwFlag = RW_FLAG::RW_END;
                PostQueuedCompletionStatus(m_hIocp, 0, completionKey, (LPOVERLAPPED)lpOverlapped);
                continue;
            }
            else
            {
                pSession->Dispatch(dwTransfer, lpOverlapped);
            }
        }
        else
        {
            H_NETWORK.PrintSockError();
        }
    }
}
