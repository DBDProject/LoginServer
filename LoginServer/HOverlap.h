#pragma once
#include "pch.h"

enum RW_FLAG
{
    NONE,
    RECV,
    SEND,
    END
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
        ZeroMemory(static_cast<OVERLAPPED*>(this), sizeof(OVERLAPPED));
        rwFlag     = RW_FLAG::NONE;
        wsabuf.buf = buffer;
        wsabuf.len = MAX_BUFFER_SIZE;
        readPos    = 0;
        writePos   = 0;
    }

    HOverlap(HOverlap&& other)           = delete;
    HOverlap& operator=(HOverlap&&)      = delete;
    HOverlap(const HOverlap&)            = delete;
    HOverlap& operator=(const HOverlap&) = delete;
};
