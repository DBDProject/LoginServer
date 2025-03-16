#include "pch.h"
#include "HPacketProcessor.h"
#include "HNetwork.h"
#include <google/protobuf/message.h>

void HPacketProcessor::Init()
{
    m_callback[HPACKET_TYPE::CHAT_MSG]            = HPacketProcessor::ProcessChatMsg;
    m_callback[HPACKET_TYPE::SEND_SURVIROR_MATCH] = HPacketProcessor::ProcessChatMsg;
    m_callback[HPACKET_TYPE::SEND_KILLER_MATCH]   = HPacketProcessor::ProcessChatMsg;
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

void HPacketProcessor::ProcessKillerMatch(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    HProtocol::MatchPlayer      packetData;
    std::shared_ptr<PlayerInfo> player    = std::make_shared<PlayerInfo>();
    std::shared_ptr<HPACKET>    ackPacket = std::make_shared<HPACKET>();

    HPacketProcessor::DeserializePacket(*packet, packetData);

    player->socket        = socket;
    player->characterType = static_cast<ECharacterType>(packetData.playercharacter());
    player->isReady       = false;

    ackPacket->ph.len  = PACKET_HEADER_SIZE;
    ackPacket->ph.type = HPACKET_TYPE::ACK_KILLER_MATCH;

    H_NETWORK.m_matching->AddKillerToMatch(player);
    H_NETWORK.m_sessionManager->GetSession(socket)->AsyncSend(ackPacket);
}

void HPacketProcessor::ProcessSurvivorMatch(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    HProtocol::MatchPlayer      packetData;
    std::shared_ptr<PlayerInfo> player    = std::make_shared<PlayerInfo>();
    std::shared_ptr<HPACKET>    ackPacket = std::make_shared<HPACKET>();

    HPacketProcessor::DeserializePacket(*packet, packetData);

    player->socket        = socket;
    player->characterType = static_cast<ECharacterType>(packetData.playercharacter());
    player->isReady       = false;

    ackPacket->ph.len  = PACKET_HEADER_SIZE;
    ackPacket->ph.type = HPACKET_TYPE::ACK_KILLER_MATCH;

    H_NETWORK.m_matching->AddSurvivorToMatch(player);
    H_NETWORK.m_sessionManager->GetSession(socket)->AsyncSend(ackPacket);
}

void HPacketProcessor::Process(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    if (m_callback.contains(packet->ph.type))
        m_callback[packet->ph.type](socket, packet);
    else
        LOG_INFO("존재하지 않는 패킷 타입 입니다.\n");
}
