#ifndef UDP_HH_
# define UDP_HH_

# include <SDL2/SDL_Net.h>
# include <string>
# include <iostream>

bool    sendUDP(UDPsocket socket, UDPpacket *packet, const char *data, Uint32 host, Uint16 port);
bool    receiveUDP(UDPsocket socket, UDPpacket *packet);

#endif
