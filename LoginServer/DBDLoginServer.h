#pragma once

#include "HNetwork.h"
#include "HTimer.h"
#include "HIocp.h"

class DBDLoginServer : public HConvention
{
private:
    HTimer m_systemTimer;
    HIocp  m_iocp;
    int    m_serverPort;

protected:
    void Init() override;
    void Release() override;

    void Update();

public:
    DBDLoginServer()  = default;
    ~DBDLoginServer() = default;

    void SetServerPort(int port);
    void Run();
};
