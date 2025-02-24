#pragma once
#include "pch.h"
#include "HThreadPool.h"
#include "HSession.h"

class HIocp : public HConvention
{
private:
    HANDLE      m_hIocp;
    HThreadPool m_threadPool;

    bool m_isRunning = true;

    std::mutex m_addPacketMutex;

public:
    HIocp()  = default;
    ~HIocp() = default;

    void Init() override;
    void Release() override;

    void WorkerProcess();
    void ProcessAsyncRecv(HSession* pSession, HOverlap* overlap, DWORD transfer);
    void ProcessAsyncSend(HSession* pSession, HOverlap* overlap, DWORD transfer);

    HANDLE GetIocpHandle() { return m_hIocp; }
};
