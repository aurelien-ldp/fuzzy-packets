#include "Server.hh"
#include <unistd.h>

int     main()
{
    Server      server(2424);

    server.listen();
    return (0);
}
