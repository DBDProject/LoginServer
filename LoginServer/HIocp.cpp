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
    ULONG_PTR CompletionKey;
    HOverlap* lpOverlapped = nullptr;

    while (m_isRunning)
    {
        BOOL ret = GetQueuedCompletionStatus(m_hIocp,
                                             &dwTransfer,
                                             &CompletionKey,
                                             (LPOVERLAPPED*)&lpOverlapped,
                                             3000);

        UserSession* pSession = (UserSession*)CompletionKey;

        //// � ������ �񵿱� �۾����� �˼��� �ִ�.
        if (pSession && lpOverlapped && lpOverlapped->rwFlag == RW_FLAG::RW_END)
        {
            H_NETWORK.m_sessionManager->DisConnect(pSession->socket);
            continue;
        }
        // if (ret == TRUE)
        //{
        //     if (dwTransfer == 0)
        //     {
        //         // ���� ���������� Ż��
        //         lpOverlapped->iIOFlag = tOV::t_END;
        //         PostQueuedCompletionStatus(iocp->g_hIOCP, 0, CompletionKey,
        //         (LPOVERLAPPED)lpOverlapped); continue;
        //     }
        //     if (pSession->Dispatch(dwTransfer, lpOverlapped))
        //     {
        //         // ���ó��, ����ó��
        //     }
        // }
        // else
        //{
        //     lpOverlapped->iIOFlag = tOV::t_END;
        //     PostQueuedCompletionStatus(iocp->g_hIOCP, 0, CompletionKey, (LPOVERLAPPED)lpOverlapped);
        // }
    }
}
