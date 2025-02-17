#include "pch.h"
#include "HThreadPool.h"

void HThreadPool::ProcessThread()
{
    std::unique_lock<std::mutex> lock(m_readyMutex);
    m_readyCondition.wait(lock, [this] { return m_isReady; });
    m_task();
}

void HThreadPool::Init()
{
    for (size_t i = 0; i < m_numThread; i++)
    {
        m_workers.push_back(std::thread(&HThreadPool::ProcessThread, this));
    }
}

void HThreadPool::Release()
{
    for (size_t i = 0; i < m_numThread; i++)
    {
        m_workers[i].join();
    }
}

void HThreadPool::SetTask(std::function<void()> task)
{
    m_task = task;
}

void HThreadPool::Run()
{
    m_isReady = true;
    m_readyCondition.notify_all();
}
