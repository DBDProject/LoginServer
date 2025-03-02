#include "pch.h"
#include "DBDLoginServer.h"
using namespace std;

int main()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    DBDLoginServer server;
    server.SetServerPort(5000);
    server.Run();
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}