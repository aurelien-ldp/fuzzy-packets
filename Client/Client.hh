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
                Client(const char* host, Uint16 port);
                ~Client();
    void        sendPacket(const char* data);
    void        receivePacket(void);
private:
    // TCP for connection
    TCPsocket   _tcpSocket;
    Uint16      _serverPort;

    // UDP for communication
    UDPsocket   _socket;
    UDPpacket*  _packet;
    Uint16      _port;
    Uint32      _host;

    // TCP server infos
	IPaddress   _servAddr;
};

#endif
