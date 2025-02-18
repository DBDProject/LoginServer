#include "pch.h"
#include "DBDLoginServer.h"
using namespace std;

int main()
{
    DBDLoginServer server;
    server.SetServerPort(5000);
    server.Run();
    return 0;
}