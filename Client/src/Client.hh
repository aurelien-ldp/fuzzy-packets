#ifndef CLIENT_HH_
# define CLIENT_HH_

# include <SDL2/SDL_Net.h>
# include <iostream>
# include <string>
# include "Common/UDP.hh"
# include "Common/TCP.hh"

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

        // UDP for the rest
        if ((_socket = SDLNet_UDP_Open(0)) == NULL)
        {
            std::cerr << "SDL_Net_UDP_Open: " << SDLNet_GetError() << std::endl;
            exit(1);
        }
        if (SDLNet_ResolveHost(&_servAddr, host, _port) == -1)
    	{
    		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", host, port, SDLNet_GetError());
    		exit(EXIT_FAILURE);
    	}
        if ((_packet = SDLNet_AllocPacket(PACKET_SIZE)) == NULL)
        {
            std::cerr << "SDL_Net_AllocPacket: " << SDLNet_GetError() << std::endl;
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
        // get port for UDP
        char buffer[16];
        if (receiveTCP(_tcpSocket, buffer, 16))
        {
            _port = SDLNet_Read16(buffer);
            std::cout << "New port: " << _port << std::endl;
        }
        IPaddress *address;

        address = SDLNet_UDP_GetPeerAddress(_socket, -1);
        this->port = address->port;
        this->host = address->host;
        char data[16];
        SDLNet_Write16(this->port, data);
        sendTCP(data, _tcpSocket);
        std::cout << "Sending: " << data << std::endl;
        char data2[32];
        SDLNet_Write32(this->host, data2);
        std::cout << "Sending: " << data2 << std::endl;
        sendTCP(data2, _tcpSocket);
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
        return (sendUDP(_socket, _packet, data, _servAddr.host, _port));
    }
    bool    receivePacket(void)
    {
        return (receiveUDP(_socket, _packet));
    }
    Uint16      port;
    Uint32      host;
private:
    Uint16      _port;
    UDPsocket   _socket;
    TCPsocket   _tcpSocket;
	IPaddress   _servAddr;
	UDPpacket*  _packet;
};

#endif
