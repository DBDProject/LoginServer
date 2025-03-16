#pragma once

enum class HPACKET_TYPE : UINT16
{
    CHAT_MSG = 0,

    SEND_SURVIROR_MATCH,
    ACK_SURVIVOR_MATCH,

    SEND_KILLER_MATCH,
    ACK_KILLER_MATCH,

    SEND_MATCH_CANCEL,
    ACK_MATCH_CANCEL,

    SEND_MATCH_READY,
    ACK_MATCH_READY,

    SEND_MAP_LOAD_END,
    ACK_MAP_LOAD_END,

    SEND_GAME_START,
    ACK_GAME_START
};

#define PACKET_HEADER_SIZE 8
#define MAX_MSG_SIZE       1024
#define MAX_BUFFER_SIZE    MAX_MSG_SIZE + PACKET_HEADER_SIZE

#pragma pack(push, 1)

typedef struct
{
    int          len;   // msg 바이트 크기
    HPACKET_TYPE type;  // 어떤 패킷이다.
    short        time;  // 패킷 전송 시간
} PACKET_HEADER;

typedef struct
{
    PACKET_HEADER ph;
    char          msg[MAX_MSG_SIZE];
} HPACKET;

#pragma pack(pop)
