#pragma once

#include "pch.h"
#include "Packet.pb.h"

class HPacketProcessor
{
private:
    inline static std::map<HPACKET_TYPE,
                           std::function<void(const SOCKET, const std::shared_ptr<HPACKET>)>>
    m_callback;

private:
    static void ProcessChatMsg(const SOCKET, const std::shared_ptr<HPACKET> packet);

public:
    static void Init();
    static void Process(const SOCKET, const std::shared_ptr<HPACKET> packet);

    template <class T>
    static bool SerializePacket(const HPACKET_TYPE packetType, const T& inSerializedData,
                                HPACKET& outPacket);
    template <class T>
    static bool DeserializePacket(const HPACKET& inPacket, T& outDeserializedData);
};

template <class T>
inline bool HPacketProcessor::SerializePacket(const HPACKET_TYPE packetType, const T& inSerializedData,
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
inline bool HPacketProcessor::DeserializePacket(const HPACKET& inPacket, T& outDeserializedData)
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
