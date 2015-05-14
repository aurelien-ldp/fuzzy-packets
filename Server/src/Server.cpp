#include "Server.hh"
#include <pthread.h>
#include <unistd.h>

// WILL be use for thread
static void*    connection_wrapper(void *arg)
{
    Server*     server_ptr = (Server *)arg;

    server_ptr->waitConnection();
    return (NULL);
}

Server::Server(Uint16 port)
{
    _port = port;
    if(SDLNet_Init()==-1)
    {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        exit(1);
    }
    // TCP for client connection
    if (SDLNet_ResolveHost(&_ipaddress, NULL, port) < 0)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        exit(1);
    }
    if ((_tcpSocket = SDLNet_TCP_Open(&_ipaddress)) == NULL)
    {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        exit(1);
    }
    // Thread the waitConnection()'s loop
    //pthread_create(&_thread, NULL, connection_wrapper, this);
    waitConnection();   // temporary
}

Server::~Server()
{
    while (!_clients.empty())
    {
        delete _clients.back();
        _clients.pop_back();
    }
    SDLNet_Quit();
}

//
// Wait for a new client connection
// and add it to the clients vector
//
void    Server::waitConnection(void)
{
    char        buffer[16];
    IPaddress*  _remoteIP;
    TCPsocket   _tcpClientSocket;

    while (1)
    {
        if ((_tcpClientSocket = SDLNet_TCP_Accept(_tcpSocket)))
        {
            if ((_remoteIP = SDLNet_TCP_GetPeerAddress(_tcpClientSocket)))
            {
                std::cout << "Host connected: " << SDLNet_Read32(&_remoteIP->host) << "\t" << SDLNet_Read16(&_remoteIP->port) << std::endl;
                _clients.push_back(new Client);
                std::cout << "New client saved, nb of client: " << _clients.size() << std::endl;
            }
            else
                std::cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << std::endl;
            SDLNet_TCP_Close(_tcpClientSocket);
        }
        usleep(200);
    }
}

void    Server::listen(void)
{
    std::cout << "Listening..." << std::endl;
    while (1)
    {
        while (!_clients.empty())
        {
            _clients[0]->receivePacket();
        }
    }
}
