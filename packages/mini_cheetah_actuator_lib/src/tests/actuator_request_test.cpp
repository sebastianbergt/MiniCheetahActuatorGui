#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <mini_cheetah_actuator_lib/actuator_request.h>
#include "mock_can.h"

constexpr auto CAN_INTERFACE_NAME{"vcan0"};

namespace sut = mini_cheetah_actuator_lib;

SCENARIO("ActuatorRequest: Sunny day")
{
    using trompeloeil::_;
    using trompeloeil::eq;
    GIVEN("A can interface")
    {
        auto mock_can = std::make_unique<MockCan>();
        auto can{mock_can.get()};

        WHEN("An ActuatorRequest is created")
        {
            sut::ActuatorRequest actuator{CAN_INTERFACE_NAME, sut::MotorId{0x01U},
                                          std::move(mock_can)};
            AND_WHEN("enable is called")
            {
                REQUIRE_CALL(*can, isConnected()).RETURN(false);
                REQUIRE_CALL(*can, connect(eq(CAN_INTERFACE_NAME))).RETURN(true);
                REQUIRE_CALL(*can, send(_)).RETURN(true);
                const auto success = actuator.enable();
                THEN("it succeeds")
                {
                    REQUIRE(success);
                }
            }
        }
    }
}

SCENARIO("ActuatorRequest: is created without dependency injection")
{
    REQUIRE_NOTHROW(sut::ActuatorRequest{CAN_INTERFACE_NAME, sut::MotorId{0x01U}});
}