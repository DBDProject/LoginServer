#pragma once
#include <WinSock2.h>

#define MAX_BUFFER_SIZE 1024

enum RW_FLAG
{
    RW_NONE,
    RW_RECV,
    RW_SEND,
    RW_END
};

struct HOverlap : public OVERLAPPED
{
    WSABUF           wsabuf;
    RW_FLAG          rwFlag;
    char             buffer[MAX_BUFFER_SIZE];
    std::atomic<int> readPos;
    std::atomic<int> writePos;

    HOverlap()
    {
        ZeroMemory(this, sizeof(*this));
        wsabuf.buf = buffer;
        wsabuf.len = MAX_BUFFER_SIZE;
        readPos    = 0;
        writePos   = 0;
    }
};
