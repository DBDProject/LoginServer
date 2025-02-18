#include "pch.h"
#include "HThreadPool.h"

void HThreadPool::ProcessThread()
{
    std::string threadId = ConvertThreadIdToStr(std::this_thread::get_id());
    {
        std::unique_lock<std::mutex> lock(m_readyMutex);
        LOG_INFO("Thread {} : Get to ready\n", threadId)
        m_readyCondition.wait(lock, [this] { return m_isReady; });
    }

    LOG_INFO("Thread {} : Start\n", threadId)
    m_task();
}

void HThreadPool::Init()
{
    LOG_INFO("ThreadPool Init\n")
    LOG_INFO("Thread Count : {}\n", m_numThread)
    for (size_t i = 0; i < m_numThread; i++)
    {
        m_workers.push_back(std::thread(&HThreadPool::ProcessThread, this));
    }
    LOG_INFO("ThreadPool Init Complete\n")
}

void HThreadPool::Release()
{
    for (size_t i = 0; i < m_numThread; i++)
    {
        m_workers[i].join();
    }
    LOG_INFO("Thread terminated\n");
}

void HThreadPool::SetTask(std::function<void()> task)
{
    m_task = task;
}

void HThreadPool::Run()
{
    LOG_INFO("ThreadPool Run\n")
    m_isReady = true;
    m_readyCondition.notify_all();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

std::string HThreadPool::ConvertThreadIdToStr(std::thread::id threadId)
{
    std::ostringstream oss;
    oss << threadId;
    return std::move(oss.str());
}
