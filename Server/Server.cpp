#include "Server.hh"
#include <unistd.h>

static void     *connection_wrapper(void *arg)
{
    Server*     server_ptr = (Server *)arg;

    server_ptr->waitConnection();
    return (nullptr);
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
    _thread = new std::thread(connection_wrapper, this);
}

Server::~Server()
{
    _thread.join();
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
void    *Server::waitConnection(void)
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
                char data[2];
                SDLNet_Write16(_clients.back()->port, data);
                while (!sendTCP(data, _tcpClientSocket));
                while (!receiveTCP(_tcpClientSocket, data, 2));
                _clients.back()->clientPort = SDLNet_Read16(data);
                char data2[4];
                while (!receiveTCP(_tcpClientSocket, data2, 4));
                _clients.back()->clientHost = SDLNet_Read16(data2);
            }
            else
                std::cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << std::endl;
            SDLNet_TCP_Close(_tcpClientSocket);
        }
        usleep(200);
    }
    return (NULL);
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
