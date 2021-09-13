#ifndef CAN_INTERFACE_LIB_I_CAN_H
#define CAN_INTERFACE_LIB_I_CAN_H

#include <string>
#include <can_interface_lib/can_frame.h>

namespace can_interface_lib
{
    class ICan
    {
    public:
        virtual ~ICan() = default;
        virtual bool connect(const std::string &interface_name) = 0;
        virtual bool disconnect() = 0;
        virtual bool send(const CanFrame &can_frame) = 0;
        virtual bool receive(CanFrame &can_frame) = 0;
    };

} // namespace strong_types

#endif // CAN_INTERFACE_LIB_I_CAN_H