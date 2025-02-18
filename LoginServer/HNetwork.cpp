#include "pch.h"
#include "HNetwork.h"

void HNetwork::InitWinSock()
{
    int ret = WSAStartup(MAKEWORD(2, 2), &m_wsadata);

    if (ret != 0)
    {
        PrintSockError();
        exit(1);
    }
}

void HNetwork::Init()
{
    m_sessionManager = std::make_shared<HSession>();
    InitWinSock();
}

void HNetwork::Release()
{
    m_sessionManager.reset();
    WSACleanup();
}

bool HNetwork::HasSockError()
{
    int errorCode = WSAGetLastError();

    switch (errorCode)
    {
    case WSAEWOULDBLOCK:
        return false;
        break;

    case ERROR_IO_PENDING:
        return false;
        break;
    }

    PrintSockError();
    return true;
}

void HNetwork::PrintSockError()
{
    int    errorCode = WSAGetLastError();
    LPVOID lpMsgBuffer;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,
                   errorCode,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPSTR)&lpMsgBuffer,
                   0,
                   NULL);
    LOG_ERROR("ERROR({}):{}\n", errorCode, (char*)lpMsgBuffer);
    LocalFree(lpMsgBuffer);
}

void HNetwork::CreateServer(std::string ip, int port)
{
    SOCKADDR_IN sa;
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    ZeroMemory(&sa, sizeof(sa));

    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port        = htons(port);
    sa.sin_family      = AF_INET;

    int namelen = sizeof(sa);
    int ret     = bind(m_serverSocket, (sockaddr*)&sa, namelen);

    if (ret < 0)
        PrintSockError();

    ret = listen(m_serverSocket, SOMAXCONN);

    if (ret < 0)
        PrintSockError();

    u_long iNonSocket = TRUE;
    int    iMode      = ioctlsocket(m_serverSocket, FIONBIO, &iNonSocket);

    LOG_INFO("========================================\n");
    LOG_INFO("Set Server IP : {} Port : {}\n", ip, port);
    LOG_INFO("========================================\n");
}

bool HNetwork::AcceptClient()
{
    SOCKADDR_IN addr;
    int         addrlen     = sizeof(addr);
    int         iMsgCounter = 0;
    SOCKET      clientSock  = accept(m_serverSocket, (sockaddr*)&addr, &addrlen);

    if (clientSock == INVALID_SOCKET)
    {
        if (HasSockError())
            return false;
    }
    else
    {
        m_sessionManager->Connect(clientSock, addr);
    }

    return true;
}
