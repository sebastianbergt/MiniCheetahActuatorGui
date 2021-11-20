#ifndef CAN_INTERFACE_LIB_MOCK_LINUX_IOCTL_H
#define CAN_INTERFACE_LIB_MOCK_LINUX_IOCTL_H

#include <linux/can.h> // sockaddr_can
#include <net/if.h>    // ifreq
#include <sys/ioctl.h> // ioctl

class MockLinuxIoctl {
public:
  MAKE_MOCK3(ioctl, int(int, unsigned long int, ifreq *));
};

#endif // CAN_INTERFACE_LIB_MOCK_LINUX_IOCTL_H