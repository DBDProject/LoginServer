#include "pch.h"
#include "HPacketProcessor.h"
#include "HNetwork.h"

void HPacketProcessor::Init()
{
    m_callback[HPACKET_TYPE::CHAT_MSG] = HPacketProcessor::ProcessChatMsg;
}

void HPacketProcessor::ProcessChatMsg(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    HProtocol::Chat packetData;

    if (!HPacketProcessor::DeserializePacket(*packet, packetData))
        return;

    std::string packetMsg = HNetAPI::ConvertUTF8ToCP949(packetData.msg());
    LOG_INFO("{}\n", packetMsg)
    H_NETWORK.m_sessionManager->Broadcast(packet);
}

void HPacketProcessor::Process(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    if (m_callback.contains(packet->ph.type))
        m_callback[packet->ph.type](socket, packet);
    else
        LOG_INFO("존재하지 않는 패킷 타입 입니다.\n");
}
