#include "pch.h"
#include "HIocp.h"

void HIocp::Init()
{
    m_threadPool.Init();
    m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_hIocp == NULL)
        H_NETWORK.PrintSockError();

    m_threadPool.SetTask(std::bind(&HIocp::WorkerProcess, this));
    m_threadPool.Run();
}

void HIocp::Release()
{
    m_threadPool.Release();
    CloseHandle(m_hIocp);
}

void HIocp::WorkerProcess()
{
    DWORD        dwTransfer;
    ULONG_PTR    CompletionKey;
    LPOVERLAPPED lpOverlapped = nullptr;

    while (m_run)
    {
        BOOL ret = GetQueuedCompletionStatus(m_hIocp,
                                             &dwTransfer,
                                             &CompletionKey,
                                             (LPOVERLAPPED*)&lpOverlapped,
                                             INFINITE);

        //// 어떤 유저의 비동기 작업인지 알수가 있다.
        // if (pSession && lpOverlapped && lpOverlapped->iIOFlag == tOV::t_END)
        //{
        //     pSession->m_bDisConnected = true;
        //     continue;
        // }
        // if (ret == TRUE)
        //{
        //     if (dwTransfer == 0)
        //     {
        //         // 유저 정상적으로 탈퇴
        //         lpOverlapped->iIOFlag = tOV::t_END;
        //         PostQueuedCompletionStatus(iocp->g_hIOCP, 0, CompletionKey,
        //         (LPOVERLAPPED)lpOverlapped); continue;
        //     }
        //     if (pSession->Dispatch(dwTransfer, lpOverlapped))
        //     {
        //         // 통계처리, 별도처리
        //     }
        // }
        // else
        //{
        //     lpOverlapped->iIOFlag = tOV::t_END;
        //     PostQueuedCompletionStatus(iocp->g_hIOCP, 0, CompletionKey, (LPOVERLAPPED)lpOverlapped);
        // }
    }
}
