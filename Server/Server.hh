#ifndef SERVER_HH_
# define SERVER_HH_

# include <SDL2/SDL_net.h>
# include <iostream>
# include <vector>
# include <thread>
# include "Client.hh"
# include "Common/TCP.hh"
# include "Common/UDP.hh"

# define PACKET_SIZE     16

class       Server
{
public:
            Server(Uint16 port);
            ~Server();
    void    *waitConnection(void);
    void    listen(void);
private:
    Uint16      _port;
    UDPsocket   _socket;
    TCPsocket   _tcpSocket;
    IPaddress   _ipaddress;
    std::thread *_thread;
    TCPsocket   _tcpClientSocket;
    std::vector <Client *>_clients;
};

#endif
