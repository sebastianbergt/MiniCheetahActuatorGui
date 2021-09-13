#ifndef CAN_INTERFACE_LIB_SOCKET_CAN_H
#define CAN_INTERFACE_LIB_SOCKET_CAN_H

#include <string>
#include <can_interface_lib/i_can.h>

namespace can_interface_lib
{
    class SocketCan : public ICan
    {
    public:
        bool connect(const std::string &interface_name) override;
        bool disconnect() override;
        bool send(const IFrame &can_frame) override;
        bool receive(IFrame &can_frame) override;

    private:
        std::int32_t socket_{};
        bool connected_{};
    };

} // namespace can_interface_lib

#endif // CAN_INTERFACE_LIB_SOCKET_CAN_H