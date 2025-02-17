#include "pch.h"
#include "HNetwork.h"

void HNetwork::Init()
{
    m_sessionManager = std::make_shared<HSession>();
}

void HNetwork::Release()
{
    m_sessionManager.reset();
}
