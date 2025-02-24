#include "pch.h"
#include "HSession.h"

HSession::HSession()
{
    ZeroMemory(&address, sizeof(address));
    socket = INVALID_SOCKET;
}

void HSession::SendPacket(const char* data, int size)
{
    HOverlap* overlap   = new HOverlap();
    overlap->rwMode     = RW_MODE::RW_WRITE;
    overlap->rwFlag     = RW_FLAG::RW_SEND;
    overlap->wsabuf.buf = const_cast<char*>(data);
    overlap->wsabuf.len = size;
    DWORD flags         = 0;
    WSASend(socket, &overlap->wsabuf, 1, nullptr, flags, overlap, nullptr);
}

void HSession::AsyncRecv()
{
#ifdef _DEBUG
    LOG_WARNING("Create overlapped\n");
#endif
    HOverlap* overlap = new HOverlap();
    overlap->rwMode   = RW_MODE::RW_READ;
    overlap->rwFlag   = RW_FLAG::RW_RECV;
    DWORD flags       = 0;
    WSARecv(socket, &overlap->wsabuf, 1, nullptr, &flags, overlap, nullptr);
}
