#pragma once

enum class TPACKET_TYPE : short
{
    PACKET_VERIFICATION_REQ = 1,
    PACKET_VERIFICATION_ACK,

    PACKET_CHAT_NAME_REQ = 1000,  // 대화명 입력요청
    PACKET_CHAT_NAME_ACK,         // 대화명 입력 응답
    PACKET_CHAT_MSG,              // 보통 채팅 메세지.
    PACKET_LOGIN_REQ,             // 로그인요청
    PACKET_LOGOUT_REQ,            // 로그아웃요청
    PACKET_LOGIN_ACK,             // 로그인응답
    PACKET_LOGOUT_ACK,            // 로그아웃응답
    PACKET_LOGOUT_PLAYER,         // 로그아웃 플레이어

    PACKET_CREATE_ACCOUNT_REQ = 2000,  // 계정생성요청
    PACKET_DELETE_ACCOUNT_REQ,         // 계정삭제요청
    PACKET_CREATE_ACCOUNT_ACK,         // 계정생성응답
    PACKET_DELETE_ACCOUNT_ACK,         // 계정삭제응답

    PACKET_USER_POSITION = 3000,  // 케릭터 위치 이동 시작
    PACKET_STOP_POSITION,         // 케릭터 정지
    PACKET_LOBBY_USER_INFO,       // 대기방 유저리스트 정보


    PACKET_CREATE_CHARACTER_REQ = 4000,  // 케릭터 생성 요청
    PACKET_DELETE_CHARACTER_REQ,         // 케릭터 삭제 요청
    PACKET_CREATE_CHARACTER_ACK,         // 케릭터 생성 응답
    PACKET_DELETE_CHARACTER_ACK,         // 케릭터 삭제 응답

    PACKET_HIT_CHARACTER = 5000,  // 케릭터 피격 받음
    PACKET_HIT_NPC,               // NPC 피격 받음
    PACKET_ATTACK_CHARACTER,      // 케릭터 공격 시작
    PACKET_ATTACK_NPC,            // NPC 공격 시작
    PACKET_DEAD_CHARACTER,        // 케릭터 죽음
    PACKET_DEAD_NPC,              // NPC 죽음
    PACKET_DAMAGE_CHARACTER,      // 케릭터 데미지 받음
    PACKET_DAMAGE_NPC,            // NPC 데미지 받음
    PACKET_SPAWN_CHARACTER,       // 케릭터 스폰
    PACKET_SPAWN_NPC,             // NPC 스폰
    PACKET_SPAWN_NPC_LIST,        //
    PACKET_SYNC_CHARACTER,        // 케릭터 동기화
    PACKET_SYNC_NPC_LIST,         // NPC 동기화
    PACKET_NPC_TARGETMOVE,        // NPC 타켓이동


    PACKET_ZONE_ENTRY_REQ = 6000,  // 게임존 입장 요청
    PACKET_ZONE_ENTRY_ACK,         // 게임 존 입장 응답
    PACKET_ZONE_NEW_PLAYER,        // 게임 존 입장 케릭터 정보 전송
    PACKET_ZONE_PLAYERS_INFO,      // 게임 존에 입장한 유저들의 플레이 케릭터 정보 전송
    PACKET_ZONE_USER_INFO,         // 게임존 유저리스트 정보(플레이어+기타(옵저버 등))
    PACKET_ZONE_TARGET_REQ,        // 존 타켓 위치 요청
    PACKET_ZONE_TARGET_ACK,        // 존 타켓 위치 응답
};

#define PACKET_HEADER_SIZE 8
#define MAX_MSG_SIZE       1024
#define MAX_BUFFER_SIZE    MAX_MSG_SIZE + PACKET_HEADER_SIZE

#pragma pack(push, 1)

typedef struct
{
    int          len;   // msg 바이트 크기
    TPACKET_TYPE type;  // 어떤 패킷이다.
    short        time;  // 패킷 전송 시간
} PACKET_HEADER;

typedef struct
{
    PACKET_HEADER ph;
    char          msg[MAX_MSG_SIZE];
} HPACKET;

#pragma pack(pop)
