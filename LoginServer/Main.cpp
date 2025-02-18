#include "pch.h"
#include "DBDLoginServer.h"
using namespace std;

int main()
{
    ServerDesc serverDesc;
    {
        serverDesc.ip   = "127.0.0.1";
        serverDesc.port = 5000;
    }

    DBDLoginServer server;
    server.SetServerDesc(serverDesc);
    server.Run();
    return 0;
}