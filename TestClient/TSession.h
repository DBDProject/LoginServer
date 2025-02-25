#pragma once
#include "TNetStd.h"
class TNetwork;
class TSession
{
public:
    SOCKET      m_sock;
    SOCKADDR_IN m_addr;
    std::string m_buf;
    UINT        m_iBeginPos = 0;
    //UINT        m_RecvByte;
    bool        m_bDisConnected;
    UPACKET     m_packet;
public:
    bool        Recv();
    void        SendPacket(UPACKET& packet);
    TSession(SOCKET sock, SOCKADDR_IN addr);
};

