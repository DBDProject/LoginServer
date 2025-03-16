#pragma once

#include "HNetwork.h"
#include "HCommand.h"

class DBDLoginServer : public HConvention
{
private:
    int m_serverPort;

protected:
    void Init() override;
    void Release() override;

public:
    DBDLoginServer()  = default;
    ~DBDLoginServer() = default;

    void InputProcess();
    void SetServerPort(int port);
    void Run();
};
