#ifndef CAN_INTERFACE_LIB_I_FRAME_H
#define CAN_INTERFACE_LIB_I_FRAME_H

#include <array>
#include <cstdint>

#include <strong_types_lib/strong_types.h>

namespace can_interface_lib
{
    constexpr std::size_t CAN_DATA_FRAME_SIZE{8U};
    using CanData = std::array<std::uint8_t, CAN_DATA_FRAME_SIZE>;
    using CanId = strong_types::StrongType<std::uint8_t, struct CanIdTag>;

    class IFrame
    {
        virtual void set(const CanData &data) = 0;
        virtual CanData &getData() = 0;
        virtual void set(const CanId &can_id) = 0;
        virtual CanId getId() = 0;
    };

} // namespace can_interface_lib

#endif // CAN_INTERFACE_LIB_I_FRAME_H