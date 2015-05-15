#include "Server.hh"
#include <pthread.h>
#include <unistd.h>

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
        exit(EXIT_FAILURE);
    }
    // TCP for client connection
    if (SDLNet_ResolveHost(&_ipaddress, NULL, port) < 0)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((_tcpSocket = SDLNet_TCP_Open(&_ipaddress)) == NULL)
    {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // UDP for the rest
    if (!(_socket = SDLNet_UDP_Open(_port)))
	{
		std::cerr << "SDLNet_UDP_Open: " << SDLNet_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
    // Thread the waitConnection()'s loop
    pthread_create(&_thread, NULL, connection_wrapper, this);
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
    IPaddress*  remoteIP;

    while (1)
    {
        if ((_tcpClientSocket = SDLNet_TCP_Accept(_tcpSocket)))
        {
            if ((remoteIP = SDLNet_TCP_GetPeerAddress(_tcpClientSocket)))
            {
                std::cout << "Host connected: " << SDLNet_Read32(&remoteIP->host) << "\t" << SDLNet_Read16(&remoteIP->port) << std::endl;
                _clients.push_back(new Client(remoteIP, _port));
                std::cout << "New client saved, nb of client: " << _clients.size() << std::endl;
                std::cout << "port of the new socket: " << _clients.back()->port << std::endl;
                char data[16];
                SDLNet_Write16(_clients.back()->port, data);
                sendTCP(data, _tcpClientSocket);
                receiveTCP(_tcpClientSocket, data, 16);
                _clients.back()->clientPort = SDLNet_Read16(data);
                char data2[32];
                receiveTCP(_tcpClientSocket, data2, 32);
                _clients.back()->clientHost = SDLNet_Read16(data2);
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
    int i;
    std::cout << "Listening..." << std::endl;
    while (1)
    {
        i = 0;
        while (i < _clients.size())
        {
            _clients[i]->receivePacket();
            _clients[i++]->sendPacket("Test");
        }
        usleep(10);
    }
}
