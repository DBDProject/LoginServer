#pragma once

// make a documentation
struct HSession
{
    sockaddr_in address;
    SOCKET      socket;

    HSession();

    void Dispatch(DWORD transfer, HOverlap* overlap);
    void SendPacket(const char* data, int size);
    void RecvPacket();
    void AsyncRecv();
};
