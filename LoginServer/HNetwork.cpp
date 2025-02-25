#include "pch.h"
#include "HNetwork.h"

/**
 * @brief Initializes the Windows Socket API
 *
 * Starts up the Winsock library with version 2.2.
 * Exits the application on failure with an error code.
 */
void HNetwork::InitWinSock()
{
    int ret = WSAStartup(MAKEWORD(2, 2), &m_wsadata);

    if (ret != 0)
    {
        PrintSockError(WSAGetLastError());
        exit(1);
    }
}

/**
 * @brief Initializes the network subsystem
 *
 * Creates a session manager, initializes WinSock, and sets up the IOCP system.
 */
void HNetwork::Init()
{
    m_sessionManager = std::make_unique<HSessionManager>();
    InitWinSock();
    m_iocp.Init();
}

/**
 * @brief Releases all network resources
 *
 * Releases IOCP resources, destroys the session manager, and cleans up WinSock.
 */
void HNetwork::Release()
{
    m_iocp.Release();
    m_sessionManager.reset();
    WSACleanup();
}

/**
 * @brief Checks if a socket error is a critical error
 *
 * @param errorCode The error code from a Winsock operation
 * @return true if the error is critical, false if it's a non-blocking operation result
 *
 * Filters out common non-critical socket errors like WSAEWOULDBLOCK and ERROR_IO_PENDING.
 */
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

    return true;
}

/**
 * @brief Formats and prints a socket error
 *
 * @param errorCode The Winsock error code to be formatted
 *
 * Formats the error code into a human-readable message and logs it.
 */
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

/**
 * @brief Creates a TCP server socket bound to the specified port
 *
 * @param port The port number on which the server will listen
 *
 * Sets up a TCP socket with non-blocking mode and disables the Nagle algorithm.
 * Binds to all available network interfaces (INADDR_ANY).
 */
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
    int errorCode;

    if (ret < 0)
    {
        errorCode = WSAGetLastError();
        H_NETWORK.PrintSockError(errorCode);
    }

    ret = listen(m_serverSocket, SOMAXCONN);

    if (ret < 0)
    {
        errorCode = WSAGetLastError();
        H_NETWORK.PrintSockError(errorCode);
    }

    u_long iNonSocket = TRUE;
    int    iMode      = ioctlsocket(m_serverSocket, FIONBIO, &iNonSocket);

    // Nagle 알고리즘 끄기 (TCP_NODELAY 설정)
    int flag = 1;
    setsockopt(m_serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

    LOG_INFO("===============================================\n");
    LOG_INFO("Server created | IP : {} Port : {}\n", GetServerIP(), port);
    LOG_INFO("===============================================\n");
}

/**
 * @brief Stops the server gracefully
 *
 * Sets the running flag to false, posts completion status to wake up all worker threads,
 * and closes the server socket.
 */
void HNetwork::StopServer()
{
    m_isRunning = false;
    for (int i = 0; i < MAX_THREAD; i++)
        PostQueuedCompletionStatus(m_iocp.GetIocpHandle(), 0, 0, nullptr);

    closesocket(m_serverSocket);
    LOG_INFO("Server stopped\n");
}

/**
 * @brief Adds a packet to the processing queue
 *
 * @param socket The socket associated with the packet
 * @param packet The packet to be added to the queue
 *
 * Adds a packet to the queue for later processing.
 */
void HNetwork::AddPacket(SOCKET socket, HPACKET* packet)
{
    if (packet)
        m_packetQueue.push(std::make_pair(socket, packet));
}

/**
 * @brief Gets the server's IP address
 *
 * @return A string containing the server's IPv4 address
 *
 * Retrieves the local hostname and resolves it to an IPv4 address.
 */
std::string HNetwork::GetServerIP()
{
    char        hostname[256];
    char        ipStr[INET_ADDRSTRLEN];
    std::string ip;

    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
    {
        LOG_ERROR("gethostname failed\n");
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
        LOG_ERROR("getaddrinfo failed\n");
        return ip;
    }

    sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(result->ai_addr);
    inet_ntop(AF_INET, &addr->sin_addr, ipStr, sizeof(ipStr));
    ip = ipStr;

    return ip;
}

/**
 * @brief Accepts a new client connection
 *
 * @return true if server can continue accepting, false if a fatal error occurred
 *
 * Accepts a new client socket connection, registers it with the IOCP,
 * and initiates an asynchronous receive operation.
 */
bool HNetwork::AcceptClient()
{
    SOCKADDR_IN addr;
    int         addrlen    = sizeof(addr);
    SOCKET      clientSock = accept(m_serverSocket, (sockaddr*)&addr, &addrlen);

    if (clientSock == INVALID_SOCKET)
    {
        int errorCode = WSAGetLastError();
        if (HasSockError(errorCode))
        {
            PrintSockError(errorCode);
            return false;
        }
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

/**
 * @brief Processes all pending packets in the queue
 *
 * Processes packets according to their type. Currently handles chat messages
 * by broadcasting them to all connected clients.
 */
void HNetwork::ProcessPacket()
{
    while (!m_packetQueue.empty())
    {
        auto [socket, packet] = m_packetQueue.front();
        m_packetQueue.pop();

        switch (packet->ph.type)
        {
        case PACKET_CHAT_MSG:
            LOG_INFO("[{}]: {}\n", socket, packet->msg);
            H_NETWORK.m_sessionManager->Broadcast(reinterpret_cast<char*>(packet), packet->ph.len);
        }
    }
}

/**
 * @brief Creates a new overlapped structure for asynchronous operations
 *
 * @return A pointer to the newly created HOverlap object
 *
 * Creates a new HOverlap object and adds it to the tracked set.
 */
HOverlap* HNetwork::AddOverlap()
{
#ifdef DEBUG_PRINT
    LOG_ERROR("AddOverlap\n");
#endif
    HOverlap* overlap = new HOverlap();
    DWORD     flags   = 0;
    {
        std::lock_guard<std::mutex> lock(m_overlapMutex);
        m_overlapSet.insert(overlap);
    }
    return overlap;
}

/**
 * @brief Deletes an overlapped structure after completion
 *
 * @param overlap The overlapped structure to delete
 * @return true if the overlap was found and deleted, false otherwise
 *
 * Removes an HOverlap object from the tracked set and frees its memory.
 */
bool HNetwork::DeleteOverlap(HOverlap* overlap)
{
#ifdef DEBUG_PRINT
    LOG_ERROR("AddOverlap\n");
#endif

    std::lock_guard<std::mutex> lock(m_overlapMutex);
    if (m_overlapSet.contains(overlap))
    {
        m_overlapSet.erase(overlap);
        delete overlap;
        return true;
    }
    return false;
}

/**
 * @brief Prints a list of all active overlapped structures
 *
 * Logs information about each overlapped structure, including its type
 * (read, write, or end) and position information.
 */
void HNetwork::PrintOverlapList()
{
    LOG_INFO("Overlap List\n");
    for (auto& overlap : m_overlapSet)
    {
        switch (overlap->rwFlag)
        {
        case RW_FLAG::RECV:
            LOG_INFO("Overlap[RD] / read : {}\n", overlap->readPos.load());
            break;

        case RW_FLAG::SEND:
            LOG_INFO("Overlap[WR] / write : {}\n", overlap->writePos.load());
            break;

        case RW_FLAG::END:
            LOG_INFO("Overlap[END]\n");
            break;
        }
    }
}
