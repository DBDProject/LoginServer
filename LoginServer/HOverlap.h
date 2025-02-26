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
    char*            buffer;
    std::atomic<int> readPos;
    std::atomic<int> writePos;

    HOverlap()
    {
        ZeroMemory(static_cast<OVERLAPPED*>(this), sizeof(OVERLAPPED));
        rwFlag     = RW_FLAG::NONE;
        buffer     = new char[MAX_BUFFER_SIZE];
        wsabuf.buf = buffer;
        wsabuf.len = MAX_BUFFER_SIZE;
        readPos    = 0;
        writePos   = 0;
    }

    ~HOverlap()
    {
        if (buffer != nullptr)
            delete[] buffer;
    }

    char* GetBuffer()
    {
        LOG_DEBUG("오버랩 버퍼 반환\n")
        return std::exchange(buffer, nullptr);
    }

    HOverlap(HOverlap&& other)           = delete;
    HOverlap& operator=(HOverlap&&)      = delete;
    HOverlap(const HOverlap&)            = delete;
    HOverlap& operator=(const HOverlap&) = delete;
};
