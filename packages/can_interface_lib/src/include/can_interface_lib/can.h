

#ifndef CAN_INTERFACE_LIB_CAN_H
#define CAN_INTERFACE_LIB_CAN_H

#include <memory>
#include <string>
#include <vector>
#include <can_interface_lib/i_can.h>
#include <can_interface_lib/i_frame.h>
#include <can_interface_lib/can_frame.h>

namespace can_interface_lib
{
    std::vector<std::string> getCanInterfaceNames();

    std::unique_ptr<ICan> makeCanInterface();

    CanFrame makeCanFrame();
} // namespace can_interface_lib

#endif // CAN_INTERFACE_LIB_CAN_H