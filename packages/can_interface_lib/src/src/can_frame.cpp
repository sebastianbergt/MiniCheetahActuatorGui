#include <can_interface_lib/can_frame.h>

namespace can_interface_lib
{
    CanId
    CanFrame::getId()
    {
        return id_;
    };
    CanData &CanFrame::getData()
    {
        return data_;
    };

    void
    CanFrame::set(const CanId &can_id)
    {
        id_ = can_id;
    }

    void
    CanFrame::set(const CanData &data)
    {
        data_ = data;
    }

} // namespace can_interface_lib
