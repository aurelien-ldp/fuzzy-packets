#include "Client.hh"
#include <unistd.h>

int     main()
{
    Client  client("localhost", 2424);
    char    str[13] = "Hello World!";

    sleep(1);
    client.sendPacket(str);
    client.sendPacket(str);
    client.sendPacket(str);
    client.sendPacket(str);
    client.receivePacket();
}
