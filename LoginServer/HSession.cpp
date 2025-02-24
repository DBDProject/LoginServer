#include "pch.h"
#include "HSession.h"

HSession::HSession()
{
    ZeroMemory(&address, sizeof(address));
    socket = INVALID_SOCKET;
}

void HSession::Dispatch(DWORD transfer, HOverlap* overlap) {}

void HSession::SendPacket(const char* data, int size) {}

void HSession::RecvPacket() {}

void HSession::AsyncRecv()
{
    HOverlap* overlap = new HOverlap();
    overlap->rwMode   = RW_MODE::RW_READ;
    overlap->rwFlag   = RW_FLAG::RW_RECV;
    DWORD flags       = 0;
    WSARecv(socket, &overlap->wsabuf, 1, nullptr, &flags, overlap, nullptr);
}
