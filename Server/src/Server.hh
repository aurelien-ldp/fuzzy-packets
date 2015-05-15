#ifndef SERVER_HH_
# define SERVER_HH_

# include <SDL2/SDL_Net.h>
# include <iostream>
# include <vector>
# include "Client.hh"
# include "Common/TCP.hh"
# include "Common/UDP.hh"

# define PACKET_SIZE     16

class       Server
{
public:
            Server(Uint16 port);
            ~Server();
    void    waitConnection(void);
    void    listen(void);
    void    sendPort(Uint16 port);
private:
    Uint16      _port;
    UDPsocket   _socket;
    TCPsocket   _tcpSocket;
    IPaddress   _ipaddress;
    pthread_t   _thread;
    TCPsocket   _tcpClientSocket;
    std::vector <Client *>_clients;
};

#endif
