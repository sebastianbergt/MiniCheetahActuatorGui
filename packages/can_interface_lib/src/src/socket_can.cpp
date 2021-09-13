#include <string>
#include <cstring>
#include <can_interface_lib/socket_can.h>

#include <unistd.h>     // close
#include <sys/socket.h> // socket, bind
#include <net/if.h>     // ifreq
#include <sys/ioctl.h>  // ioctl
#include <linux/can.h>  // sockaddr_can

namespace can_interface_lib
{
    bool SocketCan::connect(const std::string &interface_name)
    {
        // open socket
        socket_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (socket_ < 0)
        {
            return false;
        }
        // get interface
        ifreq interface_index{};
        strcpy(interface_index.ifr_name, interface_name.c_str());
        auto result = ioctl(socket_, SIOCGIFINDEX, &interface_index);
        if (result < 0)
        {
            return false;
        }
        // bind socket
        const sockaddr_can socketcan_config{.can_family = AF_CAN, .can_ifindex = interface_index.ifr_ifindex};
        result = bind(socket_, reinterpret_cast<const sockaddr *>(&socketcan_config), sizeof(socketcan_config));
        if (result < 0)
        {
            return false;
        }
        // done
        connected_ = true;
        return true;
    }

    bool SocketCan::disconnect()
    {
        if (!connected_)
        {
            return false;
        }
        if (close(socket_) < 0)
        {
            return false;
        }
        return true;
    }

    bool SocketCan::send(const IFrame &can_frame)
    {
        if (!connected_)
        {
            return false;
        }

        return true;
    }

    bool SocketCan::receive(IFrame &can_frame)
    {
        if (!connected_)
        {
            return false;
        }
        return true;
    }

} // namespace strong_types