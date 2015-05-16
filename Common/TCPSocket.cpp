#include "TCPSocket.hpp"

TCPSocket::TCPSocket(TCPSocket const &socket)
{
    this->_socket = socket._socket;
    this->_host = socket._host;
    this->_port = socket._port;
    this->_ipAddress = socket._ipAddress;
}

TCPSocket::TCPSocket(TCPsocket const socket)
{
    this->_socket = socket;
}

TCPSocket::TCPSocket(std::string const &host, short port)
{
    _host = host;
    _port = port;
    _socket = NULL;

    if (SDLNet_ResolveHost(&_ipAddress, host.c_str(), port) < 0)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((_socket = SDLNet_TCP_Open(&_ipAddress)) == NULL)
    {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

TCPSocket       &TCPSocket::accept(void)
{
    TCPsocket   clientSocket;
    clientSocket = SDLNet_TCP_Accept(_socket);
    return (TCPSocket tmp(clientSocket));
}

TCPSocket&  TCPSocket::operator=(TCPsocket socket)
{
    this->_socket = socket;
    return (*this);
}

TCPSocket&  TCPSocket::operator=(TCPSocket const &socket)
{
    this->_socket = socket._socket;
    this->_host = socket._host;
    this->_port = socket._port;
    this->_ipAddress = socket._ipAddress;
    return (*this);
}
