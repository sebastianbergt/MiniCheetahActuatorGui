#ifndef CAN_INTERFACE_LIB_CAN_FRAME_H
#define CAN_INTERFACE_LIB_CAN_FRAME_H

#include <array>
#include <inttypes>

#include <can_interface_lib/i_frame.h>

namespace can_interface_lib
{
    class CanFrame : public IFrame
    {
    private:
        std::uint32_t id{};
        std::array<std::uint8_t, 8> data{};
    }

} // namespace strong_types

#endif // CAN_INTERFACE_LIB_CAN_FRAME_H