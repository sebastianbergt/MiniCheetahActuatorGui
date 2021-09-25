#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <mini_cheetah_actuator_lib/actuator_request.h>
#include "mock_can.h"

constexpr auto CAN_INTERFACE_NAME{"vcan0"};

namespace sut = mini_cheetah_actuator_lib;

SCENARIO("ActuatorRequest: Lifecycle")
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
                const auto enable_success = actuator.enable();
                THEN("it succeeds")
                {
                    REQUIRE(enable_success);
                    AND_WHEN("setCurrentPositionToZero is called")
                    {
                        REQUIRE_CALL(*can, send(_)).RETURN(true);
                        const auto zero_pos_success = actuator.setCurrentPositionToZero();
                        THEN("it succeeds")
                        {
                            REQUIRE(zero_pos_success);
                        }
                    }

                    AND_WHEN("disable is called and returns true")
                    {
                        REQUIRE_CALL(*can, send(_)).RETURN(true);
                        const auto disable_success = actuator.disable();
                        THEN("it succeeds")
                        {
                            REQUIRE(disable_success);
                        }
                    }
                }
            }

            AND_WHEN("enable is called and fails")
            {
                REQUIRE_CALL(*can, isConnected()).RETURN(false);
                REQUIRE_CALL(*can, connect(eq(CAN_INTERFACE_NAME))).RETURN(false);
                FORBID_CALL(*can, send(_));
                const auto success = actuator.enable();
                THEN("it fails")
                {
                    REQUIRE_FALSE(success);
                }
            }
        }
    }
}

SCENARIO("ActuatorRequest: is created without dependency injection")
{
    REQUIRE_NOTHROW(sut::ActuatorRequest{CAN_INTERFACE_NAME, sut::MotorId{0x01U}});
}

SCENARIO("ActuatorRequest: Helper functions for bit banging")
{
    const float X_VALUE{GENERATE(-1E6F, -1.5F, -0.0F, 0.0F, 1.0F, 1E6F)};
    constexpr float X_MIN{1.0F};
    constexpr float X_MAX{10.0F};
    const std::uint32_t BITS_USED{GENERATE(3U, 5U, 12U, 23U, 31U)};
    const auto integer_value = sut::toInt(X_VALUE, X_MIN, X_MAX, BITS_USED);
    const auto roundtrip_value = sut::toFloat(integer_value, X_MIN, X_MAX, BITS_USED);

    if (X_VALUE <= X_MIN)
    {
        REQUIRE(roundtrip_value == X_MIN);
    }
    else if (X_VALUE >= X_MAX)
    {
        REQUIRE(roundtrip_value == X_MAX);
    }
    else
    {
        REQUIRE(Approx(roundtrip_value) == X_VALUE);
    }
}
