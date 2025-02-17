#include "pch.h"
#include "HThread.h"

HThread::HThread()
    : m_isStarted(false)
{}

void HThread::CreateThread()
{
    if (m_isStarted)
        return;

    m_isStarted = true;
    m_thread    = std::thread(&HThread::Run, this);
}
