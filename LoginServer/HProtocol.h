#pragma once

enum class HPACKET_TYPE : short
{
    CHAT_MSG = 0
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
