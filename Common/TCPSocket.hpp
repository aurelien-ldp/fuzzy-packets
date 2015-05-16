#ifndef SOCKET_HPP_
# define SOCKET_HPP_

# include <SDL2/SDL_Net.h>
# include <string>
# include <iostream>

# include "cstdlib"

class       TCPSocket
{
public:
    TCPSocket(TCPSocket const &socket);
    TCPSocket(TCPsocket const socket);
    TCPSocket(std::string const &host, short port);
    TCPSocket&      operator=(TCPsocket socket);
    TCPSocket&      operator=(TCPSocket const &socket);
    TCPSocket       &accept(void);

    std::string     _host;
    short           _port;
    IPaddress       _ipAddress;
    TCPsocket       _socket;
private:
};

#endif
