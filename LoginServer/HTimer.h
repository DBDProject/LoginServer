#pragma once

#include "pch.h"

class HTimer
{
private:
    time_point<steady_clock> m_startTimeTick;
    time_point<steady_clock> m_currentTimeTick;
    duration<double>         m_oneSecond;
    duration<double>         m_secondTime;
    duration<double>         m_frameDuration;
    duration<double>         m_elapsedTime;
    duration<double>         m_deltaTime;

    int m_fps;

public:
    HTimer();

    float GetDeltaTime() const;
    float GetElapsedTime() const;

    void Update();
    void Reset();
};
