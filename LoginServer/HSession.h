#pragma once

#include "pch.h"

struct HSession
{
    sockaddr_in address;
    SOCKET      socket;

    char readPacket[MAX_BUFFER_SIZE];
    char sendPacket[MAX_BUFFER_SIZE];

    HSession();

    void AsyncSend(const char* data, int size);
    void AsyncRecv();
};
