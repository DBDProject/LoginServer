#pragma once
#include "pch.h"
#include "HOverlap.h"
#include "HThreadPool.h"

class HIocp : public HConvention
{
private:
    HANDLE      m_hIocp;
    HThreadPool m_threadPool;

    bool m_isRunning = true;

public:
    HIocp()  = default;
    ~HIocp() = default;

    void Init() override;
    void Release() override;

    void WorkerProcess();

    HANDLE GetIocpHandle() { return m_hIocp; }
};
