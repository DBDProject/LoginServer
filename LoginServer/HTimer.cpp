#include "pch.h"
#include "HTimer.h"

HTimer::HTimer()
{
    Reset();
}

float HTimer::GetDeltaTime() const
{
    return static_cast<float>(m_deltaTime.count());
}

float HTimer::GetElapsedTime() const
{
    return static_cast<float>(m_elapsedTime.count());
}

void HTimer::Update()
{
    m_currentTimeTick  = steady_clock::now();
    m_frameDuration    = m_currentTimeTick - m_startTimeTick;
    m_deltaTime        = m_frameDuration - m_elapsedTime;
    m_secondTime      += m_deltaTime;
    m_elapsedTime      = m_frameDuration;


    if (m_secondTime >= m_oneSecond)
    {
        m_fps         = 0;
        m_secondTime -= m_oneSecond;
    }

    m_fps++;
}

void HTimer::Reset()
{
    m_startTimeTick = steady_clock::now();
    m_oneSecond     = std::chrono::seconds(1);
    m_secondTime    = duration<double>(0);
    m_elapsedTime   = duration<double>(0);
    m_frameDuration = duration<double>(0);
    m_deltaTime     = duration<double>(0);
    m_fps           = 0;
}
