#pragma once

#include "HNetwork.h"
#include "HTimer.h"
#include "HIocp.h"

struct ServerDesc
{
    std::string ip;
    int         port;

    ServerDesc() = default;
};

class DBDLoginServer : public HConvention
{
private:
    HTimer     m_systemTimer;
    HIocp      m_iocp;
    ServerDesc m_serverDesc;

protected:
    void Init() override;
    void Release() override;

    void Update();

public:
    DBDLoginServer()  = default;
    ~DBDLoginServer() = default;

    void SetServerDesc(const ServerDesc& serverDesc);
    void Run();
};
