#ifndef CAN_INTERFACE_LIB_MOCK_LINUX_IFADDRS_H
#define CAN_INTERFACE_LIB_MOCK_LINUX_IFADDRS_H

#include <unistd.h>     // close
#include <sys/socket.h> // socket, bind

#include <sys/types.h> // AF_CAN
#include <ifaddrs.h>   // getifaddrs, freeifaddrs, ifaddrs

class MockLinuxIfaddrs
{
public:
    MAKE_MOCK1(getifaddrs, int(ifaddrs *));
    MAKE_MOCK1(freeifaddrs, void(ifaddrs *));
};

#endif // CAN_INTERFACE_LIB_MOCK_LINUX_IFADDRS_H