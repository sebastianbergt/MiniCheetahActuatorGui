#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <cmath>
#include <tuple>
#include <mini_cheetah_actuator_lib/actuator.h>
#include "mock_can.h"

constexpr const char *CAN_INTERFACE_NAME{"vcan0"};
constexpr std::uint16_t MOTOR_ID{0x01U};
constexpr std::uint16_t SOME_OTHER_MOTOR_ID{0x10U};

namespace sut = mini_cheetah_actuator_lib;

SCENARIO("Actuator: is created without dependency injection")
{
    REQUIRE_NOTHROW(sut::Actuator{CAN_INTERFACE_NAME, sut::MotorId{MOTOR_ID}});
}

SCENARIO("Actuator: Lifecycle")
{
    using trompeloeil::_;
    using trompeloeil::eq;
    GIVEN("A can interface")
    {
        auto mock_can = std::make_unique<MockCan>();
        auto can{mock_can.get()};

        WHEN("An Actuator is created")
        {
            auto actuator = std::make_unique<sut::Actuator>(CAN_INTERFACE_NAME, sut::MotorId{MOTOR_ID},
                                                            std::move(mock_can));

            AND_WHEN("enable is called")
            {
                REQUIRE_CALL(*can, isConnected()).RETURN(false);
                REQUIRE_CALL(*can, connect(eq(CAN_INTERFACE_NAME))).RETURN(true);
                REQUIRE_CALL(*can, send(_)).RETURN(true);
                const auto enable_success = actuator->enable();
                THEN("it succeeds")
                {
                    REQUIRE(enable_success);
                    AND_WHEN("setCurrentPositionToZero is called")
                    {
                        REQUIRE_CALL(*can, send(_)).RETURN(true);
                        const auto zero_pos_success = actuator->setCurrentPositionToZero();
                        THEN("it succeeds")
                        {
                            REQUIRE(zero_pos_success);
                        }
                    }

                    AND_WHEN("disable is called and returns true")
                    {
                        REQUIRE_CALL(*can, send(_)).RETURN(true);
                        const auto disable_success = actuator->disable();
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
                const auto success = actuator->enable();
                THEN("it fails")
                {
                    REQUIRE_FALSE(success);
                }
            }

            REQUIRE_CALL(*can, disconnect()).RETURN(true);
            actuator.reset();
        }
    }
}

SCENARIO("Actuator: Sending commands")
{
    using trompeloeil::_;
    using trompeloeil::eq;
    GIVEN("An initialized Actuator and enabled actuator")
    {
        auto mock_can = std::make_unique<MockCan>();
        auto can{mock_can.get()};
        auto actuator = std::make_unique<sut::Actuator>(CAN_INTERFACE_NAME, sut::MotorId{MOTOR_ID},
                                                        std::move(mock_can));
        REQUIRE_CALL(*can, isConnected()).RETURN(false);
        REQUIRE_CALL(*can, connect(eq(CAN_INTERFACE_NAME))).RETURN(true);
        REQUIRE_CALL(*can, send(_)).RETURN(true);

        const auto enable_success = actuator->enable();
        REQUIRE(enable_success);

        WHEN("setPosition is called with all parameters with radiant units")
        {
            REQUIRE_CALL(*can, send(_)).RETURN(true);
            const auto success = actuator->setPosition(sut::AngleRad{M_PI}, sut::VelocityRadPerSecond{1.0F}, sut::PositionFeedbackGain{1.0F}, sut::VelocityFeedbackGain{0.1F}, sut::FeedForwardCurrentAmpere{0.0F});
            THEN("it succeeds") { CHECK(success); }
        }

        WHEN("setPosition is called with all parameters with degree units")
        {
            REQUIRE_CALL(*can, send(_)).RETURN(true);
            const auto success = actuator->setPosition(sut::AngleDeg{180.0F}, sut::VelocityDegPerSecond{180.0F}, sut::PositionFeedbackGain{1.0F}, sut::VelocityFeedbackGain{0.1F}, sut::FeedForwardCurrentAmpere{0.0F});
            THEN("it succeeds") { CHECK(success); }
        }

        AND_GIVEN("setting potentially static parameters")
        {
            // either
            actuator->configure(sut::VelocityDegPerSecond{180.0F});
            // or
            actuator->configure(sut::VelocityRadPerSecond{M_PI / 180.F}); // overwrites first

            actuator->configure(sut::PositionFeedbackGain{1.0F});
            actuator->configure(sut::VelocityFeedbackGain{0.1F});
            actuator->configure(sut::FeedForwardCurrentAmpere{0.1F});
            WHEN("setPosition is called with AngleRad")
            {
                REQUIRE_CALL(*can, send(_)).RETURN(true);
                const auto success = actuator->setPosition(sut::AngleRad{M_PI});
                THEN("it succeeds") { CHECK(success); }
            }

            WHEN("setPosition is called with AngleRad and VelocityRadPerSecond")
            {
                REQUIRE_CALL(*can, send(_)).RETURN(true);
                const auto success = actuator->setPosition(sut::AngleRad{M_PI}, sut::VelocityRadPerSecond{M_PI / 180.F});
                THEN("it succeeds") { CHECK(success); }
            }

            WHEN("setPosition is called with AngleDeg")
            {
                REQUIRE_CALL(*can, send(_)).RETURN(true);
                const auto success = actuator->setPosition(sut::AngleDeg{60.0F});
                THEN("it succeeds") { CHECK(success); }
            }

            WHEN("setPosition is called with AngleDeg and VelocityDegPerSecond")
            {
                REQUIRE_CALL(*can, send(_)).RETURN(true);
                const auto success = actuator->setPosition(sut::AngleDeg{60.0F}, sut::VelocityDegPerSecond{180.F});
                THEN("it succeeds") { CHECK(success); }
            }
        }

        REQUIRE_CALL(*can, disconnect()).RETURN(true);
        actuator.reset();
    }
}

SCENARIO("Actuator: getStatus")
{
    using trompeloeil::_;
    using trompeloeil::eq;
    GIVEN("An initialized Actuator and enabled actuator")
    {
        auto mock_can = std::make_unique<MockCan>();
        auto can{mock_can.get()};
        auto actuator = std::make_unique<sut::Actuator>(CAN_INTERFACE_NAME, sut::MotorId{MOTOR_ID},
                                                        std::move(mock_can));
        REQUIRE_CALL(*can, isConnected()).RETURN(false);
        REQUIRE_CALL(*can, connect(eq(CAN_INTERFACE_NAME))).RETURN(true);
        REQUIRE_CALL(*can, send(_)).RETURN(true);

        const auto enable_success = actuator->enable();
        REQUIRE(enable_success);

        sut::Status status{
            sut::MotorId{0x00},
            sut::AngleRad{0.0F},
            sut::VelocityRadPerSecond{0.0F},
            sut::CurrentAmpere{0.0F}};

        WHEN("getStatus receives correct message")
        {
            REQUIRE_CALL(*can, receive(_)).LR_SIDE_EFFECT(_1.data[0] = MOTOR_ID).RETURN(true);

            const auto success = actuator->getStatus(status);
            THEN("it succeeds") { CHECK(success); }
        }

        WHEN("getStatus receives an incorrect message")
        {
            REQUIRE_CALL(*can, receive(_)).LR_SIDE_EFFECT(_1.data[0] = SOME_OTHER_MOTOR_ID).RETURN(true);

            const auto result = actuator->getStatus(status);
            THEN("it fails") { CHECK_FALSE(result); }
        }

        WHEN("getStatus receiving fails")
        {
            REQUIRE_CALL(*can, receive(_)).RETURN(false);

            const auto result = actuator->getStatus(status);
            THEN("it fails") { CHECK_FALSE(result); }
        }

        REQUIRE_CALL(*can, disconnect()).RETURN(true);
        actuator.reset();
    }
}
