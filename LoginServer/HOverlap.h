#pragma once
#include "pch.h"

#define MAX_BUFFER_SIZE 1024

enum RW_MODE
{
    RW_READ,
    RW_WRITE,
};

enum RW_FLAG
{
    RW_NONE,
    RW_RECV,
    RW_SEND,
    RW_END
};

struct HOverlap : public OVERLAPPED
{
    WSABUF  wsabuf;
    RW_MODE rwMode;
    RW_FLAG rwFlag;
    char    buffer[MAX_BUFFER_SIZE];
    int     rwSize;
    int     rwCount;
    int     rwOffset;

    HOverlap()
    {
        ZeroMemory(this, sizeof(*this));
        wsabuf.buf = buffer;
        wsabuf.len = MAX_BUFFER_SIZE;
    }
};
