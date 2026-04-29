#include <fcntl.h>
#include "nonblock.h"

int set_nonblock(int fd)
{
        int flags = fcntl(fd, F_GETFL, 0);
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

