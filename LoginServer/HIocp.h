#pragma once
#include "pch.h"
#include "HThreadPool.h"
#include "HNetwork.h"

class HIocp : public HConvention
{
private:
    HANDLE      m_hIocp;
    HThreadPool m_threadPool;

    bool m_isRunning = true;

public:
    void Init() override;
    void Release() override;

    void WorkerProcess();
};
