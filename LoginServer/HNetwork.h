#pragma once

#include "pch.h"
#include "HSessionManager.h"
#include "HOverlap.h"
#include "HIocp.h"

#define H_NETWORK HNetwork::GetInstance()

class HNetwork : public HSingleton<HNetwork>, HConvention
{
private:
    WSADATA m_wsadata;
    SOCKET  m_serverSocket;
    HIocp   m_iocp;

    std::queue<std::pair<SOCKET, HPACKET*>> m_packetQueue;
    std::set<HOverlap*>                     m_overlapSet;

    std::mutex m_overlapMutex;

    H_SINGLETON_DECLARE(HNetwork)

public:
    std::unique_ptr<HSessionManager> m_sessionManager;

    inline static bool m_isRunning = true;

private:
    void InitWinSock();

public:
    void Init() override;
    void Release() override;

    void PrintSockError(int errorCode);
    bool HasSockError(int errorCode);

    bool AcceptClient();
    void CreateServer(int port);
    void StopServer();

    void AddPacket(SOCKET socket, HPACKET* packet);
    void ProcessPacket();

    HOverlap* AddOverlap();
    bool      DeleteOverlap(HOverlap* overlap);
    void      PrintOverlapList();

    std::string GetInternalServerIP();
    std::string GetExternalServerIP();

    template <class T>
        requires std::is_base_of_v<google::protobuf::Message, T>
    static bool SerializePacket(const TPACKET_TYPE packetType, const T& inSerializedData,
                                HPACKET& outPacket);
    template <class T>
        requires std::is_base_of_v<google::protobuf::Message, T>
    static bool DeserializePacket(const HPACKET& inPacket, T& outDeserializedData);
};

template <class T>
    requires std::is_base_of_v<google::protobuf::Message, T>
inline bool HNetwork::SerializePacket(const TPACKET_TYPE packetType, const T& inSerializedData,
                                      HPACKET& outPacket)
{
    std::string serialized(inSerializedData.ByteSizeLong(), '\0');
    if (!inSerializedData.SerializeToString(&serialized))
    {
        LOG_ERROR("Failed to serialize packet\n");
        return false;
    }

    if (serialized.size() > MAX_BUFFER_SIZE)
    {
        LOG_ERROR("Data size exceeds buffer limit\n");
        return false;
    }
    outPacket.ph.len  = PACKET_HEADER_SIZE + static_cast<int>(serialized.size());
    outPacket.ph.type = packetType;
    memcpy(outPacket.msg, serialized.c_str(), serialized.size());
    return true;
}

template <class T>
    requires std::is_base_of_v<google::protobuf::Message, T>
inline bool HNetwork::DeserializePacket(const HPACKET& inPacket, T& outDeserializedData)
{
    int packetSize = inPacket.ph.len - PACKET_HEADER_SIZE;

    if (!outDeserializedData.ParseFromArray(inPacket.msg, packetSize))
    {
        LOG_ERROR("Failed to deserialize packet\n");
        return false;
    }

    LOG_DEBUG("Deserialized packet\n");
    return true;
}
