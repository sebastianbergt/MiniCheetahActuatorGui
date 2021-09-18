#include <can_interface_lib/can.h>
#include <can_interface_lib/socket_can.h>
#include <can_interface_lib/can_frame.h>

#include <memory>
namespace can_interface_lib
{
    std::unique_ptr<ICan> makeCanInterface()
    {
        return std::make_unique<SocketCan>();
    }

    CanFrame makeCanFrame()
    {
        return CanFrame{};
    }
}