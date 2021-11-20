#ifndef CAN_INTERFACE_LIB_I_CAN_H
#define CAN_INTERFACE_LIB_I_CAN_H

#include <can_interface_lib/can_frame.h>
#include <string>

namespace can_interface_lib {
class ICan {
public:
  virtual ~ICan() = default;
  virtual bool connect(const std::string &interface_name) = 0;
  virtual bool disconnect() = 0;
  virtual bool isConnected() = 0;
  virtual bool send(const CanFrame &can_frame) = 0;
  virtual bool filter(CanId can_id, CanMask can_mask) = 0;
  virtual bool receive(CanFrame &can_frame) = 0;
};

} // namespace can_interface_lib

#endif // CAN_INTERFACE_LIB_I_CAN_H#