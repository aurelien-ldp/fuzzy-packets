#ifndef CLIENT_HH_
# define CLIENT_HH_

# include <SDL2/SDL_Net.h>
# include <iostream>

# define PACKET_SIZE    16

class   Client
{
public:
    Client();
    ~Client();
    bool        receivePacket(void);
    bool        sendPacket(const char *);
private:
};

#endif
