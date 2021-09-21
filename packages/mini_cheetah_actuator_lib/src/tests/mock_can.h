#ifndef MINI_CHEETAH_ACTUATOR_LIB_MOCK_CAN_H
#define MINI_CHEETAH_ACTUATOR_LIB_MOCK_CAN_H

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <can_interface_lib/i_can.h>

class MockCan : public trompeloeil::mock_interface<can_interface_lib::ICan>
{
    IMPLEMENT_MOCK1(connect);
    IMPLEMENT_MOCK0(disconnect);
    IMPLEMENT_MOCK0(isConnected);
    IMPLEMENT_MOCK1(send);
    IMPLEMENT_MOCK2(filter);
    IMPLEMENT_MOCK1(receive);
};

#endif // MINI_CHEETAH_ACTUATOR_LIB_MOCK_CAN_H