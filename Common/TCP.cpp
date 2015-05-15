#include "TCP.hh"

bool
sendTCP(const char *data, TCPsocket socket)
{
    int len = std::strlen(data) + 1;
	if (SDLNet_TCP_Send(socket, data, len) < len)
	{
		std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
		return (false);
	}
    return (true);
}

bool
receiveTCP(TCPsocket socket, char *buffer, int size)
{
    if (SDLNet_TCP_Recv(socket, buffer, size) > 0)
        return (true);
    return (false);
}
