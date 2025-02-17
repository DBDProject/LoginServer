#pragma once

#include "pch.h"
#include "HSingleton.h"
#include "HSession.h"

#define H_NETWORK HNetwork::GetInstance()

class HNetwork : public HSingleton<HNetwork>
{
private:
    H_SINGLETON_DECLARE(HNetwork)

public:
    std::shared_ptr<HSession> m_sessionManager;

public:
    void Init();
    void Release();
};
