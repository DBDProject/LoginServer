#include "pch.h"
#include "HSession.h"
#include "HNetwork.h"

HSession::HSession()
{
    ZeroMemory(&address, sizeof(address));
    socket = INVALID_SOCKET;
}

void HSession::AsyncSend(const HPACKET* inPacket)
{
    if (inPacket->ph.len > MAX_BUFFER_SIZE)
    {
        LOG_ERROR("Error : Data size exceeds buffer limit")
        return;
    }

    LOG_DEBUG("Send 오버랩 생성\n")
    HOverlap* overlap = H_NETWORK.AddOverlap();
    memcpy(overlap->buffer, reinterpret_cast<const char*>(inPacket), inPacket->ph.len);

    overlap->rwFlag     = RW_FLAG::SEND;
    overlap->wsabuf.buf = overlap->buffer;
    overlap->wsabuf.len = inPacket->ph.len;
    DWORD flags         = 0;

    int ret = WSASend(socket, &overlap->wsabuf, 1, nullptr, flags, overlap, nullptr);

    if (ret == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();

        if (H_NETWORK.HasSockError(errorCode))
        {
            H_NETWORK.PrintSockError(errorCode);
            H_NETWORK.m_sessionManager->DisConnect(socket);
            H_NETWORK.DeleteOverlap(overlap);
            LOG_DEBUG("Send 오버랩 삭제\n")
        }
    }
}

void HSession::AsyncRecv()
{
    LOG_DEBUG("Recv 오버랩 생성\n")
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
            H_NETWORK.DeleteOverlap(overlap);
            LOG_DEBUG("Recv 오버랩 삭제\n")
        }
    }
}
