#include <can_interface_lib/can.h>
#include <can_interface_lib/socket_can.h>
#include <can_interface_lib/can_frame.h>

#include <unistd.h>     // close
#include <sys/socket.h> // socket, bind
#include <net/if.h>     // ifreq
#include <sys/ioctl.h>  // ioctl
#include <linux/can.h>  // sockaddr_can
#include <linux/can/netlink.h>
#include <linux/netlink.h>
#include <array>
#include <iostream>
namespace can_interface_lib
{
    std::vector<std::string> getCanInterfaceNames()
    {
        std::vector<std::string> interface_names{};

        // open socket
        auto temp_socket = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
        if (temp_socket < 0)
        {
            return {};
        }
        // prepare query
        constexpr std::size_t MAX_INTERFACES{30U};
        std::array<ifreq, MAX_INTERFACES> interfaces{};
        ifconf interface_config;
        interface_config.ifc_len = sizeof(ifreq) * MAX_INTERFACES;
        interface_config.ifc_buf = reinterpret_cast<caddr_t>(interfaces.data());
        // query interfaces
        if (ioctl(temp_socket, SIOCGIFCONF, &interface_config) < 0)
        {
            close(temp_socket);
            return {};
        }

        for (const auto &interface : interfaces)
        {
            if (interface.ifr_addr.sa_family != AF_UNSPEC)
            {
                std::cerr << interface.ifr_name << ": " << interface.ifr_addr.sa_family << "\n";
            }

            // if (interface.ifr_addr.sa_family == )
            // {
            //const std::string name{interface.ifr_name};
            //interface_names.emplace_back(std::move(name));
            // }
        }

        close(temp_socket);

        return std::move(interface_names);
    }

    std::unique_ptr<ICan> makeCanInterface()
    {
        return std::make_unique<SocketCan>();
    }

    CanFrame makeCanFrame()
    {
        return CanFrame{};
    }
}