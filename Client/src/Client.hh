#ifndef CLIENT_HH_
# define CLIENT_HH_

# include <SDL2/SDL_Net.h>
# include <iostream>
# include <string>

# define PACKET_SIZE    512

class           Client
{
public:
    Client(const char* host, Uint16 port)
    {
        if (SDLNet_Init() < 0)
        {
            std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
            exit(1);
        }

        // TCP for connection
        if (SDLNet_ResolveHost(&_servAddr, host, port) < 0)
        {
            std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
            exit(1);
        }
        if ((_tcpSocket = SDLNet_TCP_Open(&_servAddr)) == NULL)
        {
            std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
            exit(1);
        }

        // UDP for the rest
        if ((_socket = SDLNet_UDP_Open(0)) == NULL)
        {
            std::cerr << "SDL_Net_UDP_Open: " << SDLNet_GetError() << std::endl;
            exit(1);
        }
        if ((_packet = SDLNet_AllocPacket(PACKET_SIZE)) == NULL)
        {
            std::cerr << "SDL_Net_AllocPacket: " << SDLNet_GetError() << std::endl;
            exit(1);
        }
    }
    ~Client()
    {
        SDLNet_TCP_Close(_tcpSocket);
        _tcpSocket = NULL;
        SDLNet_UDP_Close(_socket);
        _socket = NULL;
        SDLNet_FreePacket(_packet);
        _packet = NULL;
        SDLNet_Quit();
    }
    bool    sendPacket(const char* data)
    {
        strcpy((char *)_packet->data, data);

        _packet->len = strlen(data) + 1;
        if (SDLNet_UDP_Send(_socket, -1, _packet) == 0)
            return (false);
        return (true);
    }
    bool    receivePacket(void)
    {
        if (SDLNet_UDP_Recv(_socket, _packet))
        {
            printf("UDP Packet incoming\n");
			printf("\tData:    %s\n", (char *)_packet->data);
            return (true);
        }
        return (false);
    }
private:
    UDPsocket   _socket;
    TCPsocket   _tcpSocket;
	IPaddress   _servAddr;
	UDPpacket*  _packet;
};

#endif
