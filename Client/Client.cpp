#include "Client.hh"

Client::Client(const char* host, Uint16 port)
{
    if (SDLNet_Init() < 0)
    {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        exit(1);
    }

    // UDP for the rest
    if ((_socket = SDLNet_UDP_Open(0)) == NULL)
    {
        std::cerr << "SDL_Net_UDP_Open: " << SDLNet_GetError() << std::endl;
        exit(1);
    }
    if (SDLNet_ResolveHost(&_servAddr, host, port) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", host, port, SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if ((_packet = SDLNet_AllocPacket(PACKET_SIZE)) == NULL)
    {
        std::cerr << "SDL_Net_AllocPacket: " << SDLNet_GetError() << std::endl;
        exit(1);
    }

    // TCP for connection
    if (SDLNet_ResolveHost(&_servAddr, host, port) < 0)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        exit(1);
    }
    if ((_tcpSocket = SDLNet_TCP_Open(&_servAddr)) == NULL)
    {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        exit(1);
    }
    // get port for UDP
    char buffer[2];
    if (receiveTCP(_tcpSocket, buffer, 16))
    {
        _serverPort = SDLNet_Read16(buffer);
        std::cout << "New port: " << _serverPort << std::endl;
    }
    IPaddress *address;

    address = SDLNet_UDP_GetPeerAddress(_socket, -1);
    _port = address->port;
    _host = address->host;
    char data[2];
    SDLNet_Write16(_port, data);
    sendTCP(data, _tcpSocket);
    std::cout << "Sending: " << data << std::endl;
    char data2[4];
    SDLNet_Write32(_host, data2);
    std::cout << "Sending: " << data2 << std::endl;
    sendTCP(data2, _tcpSocket);
}

Client::~Client()
{
    SDLNet_TCP_Close(_tcpSocket);
    _tcpSocket = NULL;
    SDLNet_UDP_Close(_socket);
    _socket = NULL;
    SDLNet_FreePacket(_packet);
    _packet = NULL;
    SDLNet_Quit();
}

void    Client::sendPacket(const char* data)
{
    while (!sendUDP(_socket, _packet, data, _servAddr.host, _serverPort));
}

void    Client::receivePacket(void)
{
    while (!receiveUDP(_socket, _packet));
}
