#ifndef CLIENT_HH_
# define CLIENT_HH_

# include <SDL2/SDL_net.h>
# include <iostream>
# include "Common/TCP.hh"
# include "Common/UDP.hh"


# define PACKET_SIZE    16

class   Client
{
public:
    Client(IPaddress *cipaddress, Uint16 serverPort);
    ~Client();
    bool        receivePacket(void);
    bool        sendPacket(const char *);
    Uint16      port;
    Uint16      clientPort;
    Uint32      clientHost;
private:
    IPaddress*  _ipaddress;
    Uint16      _serverPort;
    UDPsocket   _socket;
    UDPpacket*  _packet;
    int         _clientNb;
};

#endif
