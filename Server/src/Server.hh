#ifndef SERVER_HH_
# define SERVER_HH_

# include <SDL2/SDL_Net.h>
# include <iostream>
# include <vector>
# include "Client.hh"

# define PACKET_SIZE     16

class       Server
{
public:
            Server(Uint16 port);
            ~Server();
    void    waitConnection(void);
    bool    receivePacket(void);
    bool    sendPacket(const char* data);
    void    listen(void);
private:
    Uint16      _port;
    TCPsocket   _tcpSocket;
    IPaddress   _ipaddress;
    pthread_t   _thread;
    std::vector <Client *>_clients;
};

#endif
