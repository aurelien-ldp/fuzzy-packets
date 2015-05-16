#include "Client.hh"
#include <unistd.h>

static int client_counter(void)
{
    static int c = 1;
    return (c++);
}

Client::Client(IPaddress *cipaddress, Uint16 serverPort)
{
    _ipaddress = cipaddress;
    _serverPort = serverPort;
    _clientNb = client_counter();

    if (!(_socket = SDLNet_UDP_Open(0)))
	{
		std::cerr << "SDLNet_UDP_Open: " << SDLNet_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
    IPaddress *address;
    address = SDLNet_UDP_GetPeerAddress(_socket, -1);
    this->port = address->port;
    if ((_packet = SDLNet_AllocPacket(PACKET_SIZE)) == NULL)
    {
        std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

Client::~Client()
{
    SDLNet_UDP_Close(_socket);
    _socket = NULL;
    SDLNet_FreePacket(_packet);
    _packet = NULL;
}

bool    Client::receivePacket(void)
{
	return (receiveUDP(_socket, _packet));
}

bool    Client::sendPacket(const char *data)
{
    return (sendUDP(_socket, _packet, data, clientHost, clientPort));
}
