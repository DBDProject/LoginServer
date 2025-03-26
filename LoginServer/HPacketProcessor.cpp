#include "pch.h"
#include "HPacketProcessor.h"
#include "HNetwork.h"
#include "HMatching.h"
#include <google/protobuf/message.h>

void HPacketProcessor::Init()
{
    m_callback[HPACKET_TYPE::CHAT_MSG]            = HPacketProcessor::ProcessChatMsg;
    m_callback[HPACKET_TYPE::SEND_SURVIROR_MATCH] = HPacketProcessor::ProcessSurvivorMatch;
    m_callback[HPACKET_TYPE::SEND_KILLER_MATCH]   = HPacketProcessor::ProcessKillerMatch;
    m_callback[HPACKET_TYPE::SEND_MATCH_CANCEL]   = HPacketProcessor::ProcessMatchCancel;
    m_callback[HPACKET_TYPE::SEND_MAP_LOAD_END]   = HPacketProcessor::ProcessMapLoadEnd;
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
    player->address       = H_NETWORK.m_sessionManager->GetSession(socket)->GetAddress();
    player->characterType = static_cast<ECharacterType>(packetData.playercharacter());
    player->isReady       = false;

    ackPacket->ph.len  = PACKET_HEADER_SIZE;
    ackPacket->ph.type = HPACKET_TYPE::ACK_KILLER_MATCH;

    LOG_INFO("[{}][Type : {}] : 킬러 매칭 요청\n", socket, (int)player->characterType);
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
    player->address       = H_NETWORK.m_sessionManager->GetSession(socket)->GetAddress();
    player->characterType = static_cast<ECharacterType>(packetData.playercharacter());
    player->isReady       = false;

    ackPacket->ph.len  = PACKET_HEADER_SIZE;
    ackPacket->ph.type = HPACKET_TYPE::ACK_SURVIVOR_MATCH;

    LOG_INFO("[{}][Type : {}] : 서바이버 매칭 요청\n", socket, (int)player->characterType);
    H_NETWORK.m_matching->AddSurvivorToMatch(player);
    H_NETWORK.m_sessionManager->GetSession(socket)->AsyncSend(ackPacket);
}

void HPacketProcessor::ProcessMatchCancel(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    std::shared_ptr<HPACKET> ackPacket = std::make_shared<HPACKET>();

    ackPacket->ph.len  = PACKET_HEADER_SIZE;
    ackPacket->ph.type = HPACKET_TYPE::ACK_MATCH_CANCEL;

    H_NETWORK.m_matching->DeleteKillerFromMatch(socket);
    H_NETWORK.m_matching->DeleteSurvivorFromMatch(socket);
    H_NETWORK.m_sessionManager->GetSession(socket)->AsyncSend(ackPacket);

    LOG_INFO("[{}] : 매칭 취소\n", socket);
}

void HPacketProcessor::ProcessMapLoadEnd(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    if (H_NETWORK.m_matching->IsMatchingPlayer(socket))
    {
        LOG_INFO("[{}] : 맵 로드 완료\n", socket);
        H_NETWORK.m_matching->ReadyPlayer(socket);
    }
}

void HPacketProcessor::Process(const SOCKET socket, const std::shared_ptr<HPACKET> packet)
{
    if (m_callback.contains(packet->ph.type))
        m_callback[packet->ph.type](socket, packet);
    else
        LOG_INFO("존재하지 않는 패킷 타입 입니다.\n");
}

void HPacketProcessor::SendChatMsg(const std::string& msg)
{
    HProtocol::Chat          packetData;
    std::shared_ptr<HPACKET> packet = std::make_shared<HPACKET>();

    packetData.set_msg(HNetAPI::ConvertCP949ToUTF8(msg));
    HPacketProcessor::SerializePacket(HPACKET_TYPE::CHAT_MSG, packetData, *packet);

    H_NETWORK.m_sessionManager->Broadcast(packet);
}

void HPacketProcessor::SendChatMsg(const SOCKET socket, const std::string& msg)
{
    HProtocol::Chat          packetData;
    std::shared_ptr<HPACKET> packet = std::make_shared<HPACKET>();

    packetData.set_msg(HNetAPI::ConvertCP949ToUTF8(msg));
    HPacketProcessor::SerializePacket(HPACKET_TYPE::CHAT_MSG, packetData, *packet);

    H_NETWORK.m_sessionManager->GetSession(socket)->AsyncSend(packet);
}

void HPacketProcessor::SendMatchReady(const MatchInfo& matchInfo)
{
    HProtocol::MatchReady    packetData;
    std::shared_ptr<HPACKET> packet = std::make_shared<HPACKET>();
    packetData.set_maxplayer(matchInfo.matchPlayer);
    packetData.set_isserver(false);
    packetData.set_killerip(HNetAPI::ConvertCP949ToUTF8(matchInfo.killer->address));
    packetData.set_killercharacter((uint32_t)matchInfo.killer->characterType);

    for (const auto& player : matchInfo.survivor)
    {
        packetData.add_survivorcharacter((uint32_t)player->characterType);
        packetData.add_survivorip(HNetAPI::ConvertCP949ToUTF8(player->address));
    }

    HPacketProcessor::SerializePacket(HPACKET_TYPE::SEND_MATCH_READY, packetData, *packet);

    for (const auto& player : matchInfo.survivor)
        H_NETWORK.m_sessionManager->GetSession(player->socket)->AsyncSend(packet);

    packetData.set_isserver(true);


    HPacketProcessor::SerializePacket(HPACKET_TYPE::SEND_MATCH_READY, packetData, *packet);
    H_NETWORK.m_sessionManager->GetSession(matchInfo.killer->socket)->AsyncSend(packet);

    LOG_INFO("===============================================\n");
    LOG_INFO("{}번방 매칭이 성립되었습니다.\n", matchInfo.matchID);
    LOG_INFO("킬러 : {} | IP : {}\n", matchInfo.killer->socket, matchInfo.killer->address);
    for (const auto& player : matchInfo.survivor)
    {
        LOG_INFO("생존자 : {} | IP : {}\n", player->socket, player->address);
    }
    LOG_INFO("===============================================\n");

    for (const auto& player : matchInfo.survivor)
    {
        SendChatMsg(player->socket, "==============================\n");
        SendChatMsg(player->socket, "매칭이 성립되었습니다. 준비하세요\n");
        SendChatMsg(player->socket, "==============================\n");
    }

    SendChatMsg(matchInfo.killer->socket, "==============================\n");
    SendChatMsg(matchInfo.killer->socket, "매칭이 성립되었습니다. 준비하세요\n");
    SendChatMsg(matchInfo.killer->socket, "==============================\n");
}

void HPacketProcessor::SendMatchAbandoned(const MatchInfo& matchInfo)
{
    std::shared_ptr<HPACKET> ackPacket = std::make_shared<HPACKET>();

    ackPacket->ph.len  = PACKET_HEADER_SIZE;
    ackPacket->ph.type = HPACKET_TYPE::SEND_MATCH_ABANDONED;

    auto* session = H_NETWORK.m_sessionManager->GetSession(matchInfo.killer->socket);

    if (session)
        session->AsyncSend(ackPacket);

    for (const auto& player : matchInfo.survivor)
    {
        session = H_NETWORK.m_sessionManager->GetSession(player->socket);
        if (session)
            session->AsyncSend(ackPacket);
    }

    LOG_INFO("{}번방의 매칭이 강제로 종료되었습니다.\n", matchInfo.matchID);
}

void HPacketProcessor::SendMatchStart(const MatchInfo& matchInfo)
{
    std::shared_ptr<HPACKET> ackPacket = std::make_shared<HPACKET>();
    ackPacket->ph.len                  = PACKET_HEADER_SIZE;
    ackPacket->ph.type                 = HPACKET_TYPE::SEND_MATCH_START;
    auto* session = H_NETWORK.m_sessionManager->GetSession(matchInfo.killer->socket);
    if (session)
        session->AsyncSend(ackPacket);
    for (const auto& player : matchInfo.survivor)
    {
        session = H_NETWORK.m_sessionManager->GetSession(player->socket);
        if (session)
            session->AsyncSend(ackPacket);
    }
    LOG_INFO("{}번방의 게임이 시작되었습니다.\n", matchInfo.matchID);
}
