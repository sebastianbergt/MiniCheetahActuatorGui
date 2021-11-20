#ifndef CAN_INTERFACE_LIB_CAN_FRAME_H
#define CAN_INTERFACE_LIB_CAN_FRAME_H

#include <array>
#include <cstdint>

#include <strong_types_lib/strong_types.h>

namespace can_interface_lib {
using CanId = strong_types::StrongType<std::uint16_t, struct CanIdTag>;
using CanMask = strong_types::StrongType<std::uint16_t, struct CanIdTag>;

constexpr std::size_t CAN_DATA_FRAME_SIZE{8U};
using CanData = std::array<std::uint8_t, CAN_DATA_FRAME_SIZE>;

using BytesUsed = strong_types::StrongType<std::uint8_t, struct BytesUsedTag>;

class CanFrame {
public:
  CanId id{0x0000};
  CanData data{};
  BytesUsed bytes_used{CAN_DATA_FRAME_SIZE};
};

} // namespace can_interface_lib

#endif // CAN_INTERFACE_LIB_CAN_FRAME_H