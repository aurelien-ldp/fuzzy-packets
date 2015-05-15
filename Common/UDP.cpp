#include "UDP.hh"

bool
sendUDP(UDPsocket socket, UDPpacket *packet, const char *data, Uint32 host, Uint16 port)
{
    std::strcpy((char *)packet->data, data);
    packet->address.host = host;
    packet->address.port = port;
    packet->len = std::strlen(data) + 1;
    if (SDLNet_UDP_Send(socket, -1, packet) == 0)
        return (false);
    return (true);
}

bool
receiveUDP(UDPsocket socket, UDPpacket *packet)
{
    if (SDLNet_UDP_Recv(socket, packet))
	{
		printf("\tChan:    %d\n", packet->channel);
		printf("\tData:    %s\n", (char *)packet->data);
		printf("\tLen:     %d\n", packet->len);
		printf("\tMaxlen:  %d\n", packet->maxlen);
		printf("\tStatus:  %d\n", packet->status);
		printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);
        return (true);
	}
    return (false);
}
