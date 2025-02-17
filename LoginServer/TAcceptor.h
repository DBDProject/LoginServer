#pragma once
#include "pch.h"
#include "HThread.h"

#define BUFFERSIZE 1024
#define NAMESIZE   30

class TAcceptor : public HThread
{
private:
    SOCKET m_ListenSock;

public:
    virtual bool Run() override;

    bool Set(int iPort, const char* strAddress = 0);
    int  GetIPList(std::vector<std::string>& ipList);
    bool Release();

public:
    TAcceptor();
    virtual ~TAcceptor();
};
