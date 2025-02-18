#pragma once
#include "pch.h"

#define MAX_THREAD 4

class HThreadPool : public HConvention
{
protected:
    std::vector<std::thread> m_workers;
    std::condition_variable  m_readyCondition;
    std::mutex               m_readyMutex;

    size_t m_numThread = MAX_THREAD;
    bool   m_isReady   = false;

    std::function<void()> m_task;

private:
    void ProcessThread();

public:
    HThreadPool()  = default;
    ~HThreadPool() = default;

    void Init() override;
    void Release() override;

    void SetTask(std::function<void()> task);
    void Run();

    static std::string ConvertThreadIdToStr(std::thread::id threadId);
};
