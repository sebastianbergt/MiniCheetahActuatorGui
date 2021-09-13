#include <unistd.h>     // close
#include <sys/socket.h> // socket, bind

class MockLinuxSocket
{
public:
    MAKE_MOCK3(socket, int(int, int, int));
    MAKE_MOCK3(bind, int(int, const struct sockaddr *, socklen_t));

    MAKE_MOCK3(read, ssize_t(int, void *, size_t));
    MAKE_MOCK3(write, ssize_t(int, const void *, size_t));

    MAKE_MOCK1(close, int(int));
};