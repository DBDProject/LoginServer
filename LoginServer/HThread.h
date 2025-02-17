#pragma once
#include <thread>

class HThread
{
protected:
    std::thread m_thread;
    bool        m_isStarted;

public:
    HThread();
    ~HThread() = default;

    void CreateThread();

    virtual bool Run() = 0;
};
