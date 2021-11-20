

#ifndef CAN_INTERFACE_LIB_CAN_H
#define CAN_INTERFACE_LIB_CAN_H

#include <can_interface_lib/can_frame.h>
#include <can_interface_lib/i_can.h>
#include <memory>

namespace can_interface_lib {
std::unique_ptr<ICan> makeCanInterface();

CanFrame makeCanFrame();
} // namespace can_interface_lib

#endif // CAN_INTERFACE_LIB_CAN_H