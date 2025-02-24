#include "pch.h"
#include "HSession.h"
#include "HNetwork.h"

HSession::HSession()
{
    ZeroMemory(&address, sizeof(address));
    socket = INVALID_SOCKET;
}

void HSession::AsyncSend(const char* data, int size)
{
    if (size > MAX_BUFFER_SIZE)
    {
        LOG_ERROR("Error : Data size exceeds buffer limit")
        return;
    }


    HOverlap* overlap = H_NETWORK.AddOverlap();
    memcpy(overlap->buffer, data, size);

    overlap->rwFlag     = RW_FLAG::SEND;
    overlap->wsabuf.buf = overlap->buffer;
    overlap->wsabuf.len = size;
    DWORD flags         = 0;

    int ret = WSASend(socket, &overlap->wsabuf, 1, nullptr, flags, overlap, nullptr);

    if (ret == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();

        if (H_NETWORK.HasSockError(errorCode))
        {
            H_NETWORK.PrintSockError(errorCode);
            H_NETWORK.m_sessionManager->DisConnect(socket);
        }
    }
}

void HSession::AsyncRecv()
{
    HOverlap* overlap = H_NETWORK.AddOverlap();
    overlap->rwFlag   = RW_FLAG::RECV;

    DWORD flags = 0;
    int   ret   = WSARecv(socket, &overlap->wsabuf, 1, nullptr, &flags, overlap, nullptr);

    if (ret == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();

        if (H_NETWORK.HasSockError(errorCode))
        {
            H_NETWORK.PrintSockError(errorCode);
            H_NETWORK.m_sessionManager->DisConnect(socket);
        }
    }
}
