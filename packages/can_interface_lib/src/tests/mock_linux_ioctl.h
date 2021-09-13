#include <net/if.h>    // ifreq
#include <sys/ioctl.h> // ioctl
#include <linux/can.h> // sockaddr_can

class MockLinuxIoctl
{
public:
    MAKE_MOCK3(ioctl, int(int, unsigned long int, ifreq *));
};