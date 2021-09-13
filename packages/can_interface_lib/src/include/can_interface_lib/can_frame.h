#ifndef CAN_INTERFACE_LIB_CAN_FRAME_H
#define CAN_INTERFACE_LIB_CAN_FRAME_H

#include <array>
#include <cstdint>

#include <can_interface_lib/i_frame.h>

namespace can_interface_lib
{
    class CanFrame : public IFrame
    {
    public:
        void set(const CanData &data) override;
        CanData &getData() override;
        void set(const CanId &can_id) override;
        CanId getId() override;

    private:
        CanId id_{0x00};
        std::array<std::uint8_t, 8> data_{};
    };

} // namespace strong_types

#endif // CAN_INTERFACE_LIB_CAN_FRAME_H