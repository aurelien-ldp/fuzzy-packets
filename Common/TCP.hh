#ifndef TCP_HH_
# define TCP_HH_

# include <SDL2/SDL_Net.h>
# include <string>
# include <iostream>

bool    sendTCP(const char *data, TCPsocket socket);
bool    receiveTCP(TCPsocket socket, char *buffer, int size);

#endif
