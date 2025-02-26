#include "TNetClient.h"

void SenderThread(TNetClient& net)
{
    std::string UserName = "[홍길동]";
    std::string Sendbuf;
    Sendbuf.reserve(256);
    std::string KeyDatabuf;
    KeyDatabuf.reserve(256);

    while (net.m_bRun)
    {
        Sendbuf.clear();
        std::cout << "데이터입력(종료:엔터) : ";
        std::getline(std::cin, KeyDatabuf);
        if (KeyDatabuf.empty())
        {
            break;
        }
        Sendbuf += KeyDatabuf;

        UPACKET packet = TNetwork::MakePacket(Sendbuf.size(), PACKET_CHAT_MSG, Sendbuf.c_str());

        net.SendPacket(packet);
    }
    closesocket(net.g_hSock);
}

int main()
{
    TNetClient net;
    net.Connected("182.226.113.68", 5000);

    // 종업원 1명
    std::thread sendThread(SenderThread, std::ref(net));
    sendThread.detach();

    net.Run();
    net.Release();
}
