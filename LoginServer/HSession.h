#pragma once

#include "pch.h"

struct HSession
{
    sockaddr_in address;
    SOCKET      socket;

    char recvPacket[MAX_BUFFER_SIZE];

    HSession();

    void AsyncSend(const std::shared_ptr<HPACKET> inPacket);
    void AsyncRecv();
};
