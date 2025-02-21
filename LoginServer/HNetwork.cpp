#include "pch.h"
#include "HNetwork.h"

void HNetwork::InitWinSock()
{
    int ret = WSAStartup(MAKEWORD(2, 2), &m_wsadata);

    if (ret != 0)
    {
        PrintSockError(WSAGetLastError());
        exit(1);
    }
}

void HNetwork::Init()
{
    m_sessionManager = std::make_unique<HSessionManager>();
    InitWinSock();
    m_iocp.Init();
}

void HNetwork::Release()
{
    m_sessionManager.reset();
    m_iocp.Release();
    WSACleanup();
}

bool HNetwork::HasSockError(int errorCode)
{
    switch (errorCode)
    {
    case WSAEWOULDBLOCK:
        return false;
        break;

    case ERROR_IO_PENDING:
        return false;
        break;
    }

    PrintSockError(errorCode);
    return true;
}

void HNetwork::PrintSockError(int errorCode)
{
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

void HNetwork::CreateServer(int port)
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
        PRINT_SOCKET_ERROR()

    ret = listen(m_serverSocket, SOMAXCONN);

    if (ret < 0)
        PRINT_SOCKET_ERROR()

    u_long iNonSocket = TRUE;
    int    iMode      = ioctlsocket(m_serverSocket, FIONBIO, &iNonSocket);

    // Nagle 알고리즘 끄기 (TCP_NODELAY 설정)
    int flag = 1;
    setsockopt(m_serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

    LOG_INFO("===============================================\n")
    LOG_INFO("Server created | IP : {} Port : {}\n", GetServerIP(), port)
    LOG_INFO("===============================================\n")
}

std::string HNetwork::GetServerIP()
{
    char        hostname[256];
    char        ipStr[INET_ADDRSTRLEN];
    std::string ip;

    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
    {
        LOG_ERROR("gethostname failed\n")
        return ip;
    }

    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family   = AF_INET;  // IPv4만
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* result = nullptr;
    if (getaddrinfo(hostname, nullptr, &hints, &result) != 0)
    {
        LOG_ERROR("getaddrinfo failed\n")
        return ip;
    }

    sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(result->ai_addr);
    inet_ntop(AF_INET, &addr->sin_addr, ipStr, sizeof(ipStr));
    ip = ipStr;

    return ip;
}

bool HNetwork::AcceptClient()
{
    SOCKADDR_IN addr;
    int         addrlen     = sizeof(addr);
    int         iMsgCounter = 0;
    SOCKET      clientSock  = accept(m_serverSocket, (sockaddr*)&addr, &addrlen);

    if (clientSock == INVALID_SOCKET)
    {
        if (HasSockError(WSAGetLastError()))
            return false;
    }
    else
    {
        m_sessionManager->Connect(clientSock, addr);
        HSession* pSession = m_sessionManager->GetSession(clientSock);
        CreateIoCompletionPort((HANDLE)clientSock, m_iocp.GetIocpHandle(), (ULONG_PTR)pSession, 0);
        pSession->AsyncRecv();
    }

    return true;
}

bool HNetwork::ProcessPactket()
{
    return true;
}
