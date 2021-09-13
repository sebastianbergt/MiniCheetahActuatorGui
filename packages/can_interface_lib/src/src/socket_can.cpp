#include <string>
#include <cstring>
#include <can_interface_lib/socket_can.h>
#include <iterator>
#include <algorithm>

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

    bool SocketCan::send(const CanFrame &can_frame_to_sent)
    {
        if (!connected_)
        {
            return false;
        }

        can_frame socket_can_frame{
            .can_id = can_frame_to_sent.id.get(),
            .can_dlc = can_frame_to_sent.bytes_used.get()};
        std::copy(can_frame_to_sent.data.cbegin(), can_frame_to_sent.data.cend(), std::begin(socket_can_frame.data));

        auto bytes_sent = write(socket_, &socket_can_frame, sizeof(can_frame));
        if (bytes_sent <= 0)
        {
            return false;
        }

        return true;
    }

    bool SocketCan::receive(CanFrame &can_frame_to_receive)
    {
        if (!connected_)
        {
            return false;
        }

        can_frame socket_can_frame;
        auto bytes_received = read(socket_, &socket_can_frame, sizeof(socket_can_frame));
        if (bytes_received <= 0)
        {
            return false;
        }
        can_frame_to_receive.id = CanId(socket_can_frame.can_id);
        can_frame_to_receive.bytes_used = CanBytesUsed(socket_can_frame.can_dlc);
        std::copy(std::begin(socket_can_frame.data), std::end(socket_can_frame.data), can_frame_to_receive.data.begin());

        return true;
    }

} // namespace strong_types