#pragma once

#include "HNetwork.h"
#include "HTimer.h"

class DBDLoginServer
{
private:
    HTimer m_systemTimer;

protected:
    void Init();
    void Update();
    void Release();

public:
    DBDLoginServer()  = default;
    ~DBDLoginServer() = default;

    void Run();
};
