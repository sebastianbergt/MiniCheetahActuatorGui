#ifndef MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_REQUEST_H
#define MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_REQUEST_H

#include <strong_types_lib/strong_types.h>
#include <cstdint>
#include <memory>
#include <string>

namespace mini_cheetah_actuator_lib
{
    using AngleDeg = strong_types::StrongType<float, struct AngleDegTag>;
    using VelocityDegPerSecond = strong_types::StrongType<float, struct VelocityDegPerSecondTag>;
    using AngleRad = strong_types::StrongType<float, struct AngleRadTag>;
    using VelocityRadPerSecond = strong_types::StrongType<float, struct AngleRadTag>;
    using PositionFeedbackGain = strong_types::StrongType<float, struct PositionFeedbackGainTag>;
    using VelocityFeedbackGain = strong_types::StrongType<float, struct VelocityFeedbackGainTag>;
    using FeedForwardCurrent = strong_types::StrongType<float, struct FeedForwardCurrentTag>;
    using MotorId = strong_types::StrongType<std::uint8_t, struct MotorIdTag>;

    class IActuatorRequest;

    std::unique_ptr<IActuatorRequest> makeActuatorRequest(std::string can_interface, MotorId motor_id);

    class IActuatorRequest
    {
    public:
        virtual bool enable() = 0;
        virtual bool disable() = 0;
        virtual bool setCurrentPositionToZero() = 0;
        // methods mapping directly to actuator CAN commands
        virtual bool setPosition(AngleDeg position, VelocityDegPerSecond velocity, PositionFeedbackGain Kp, VelocityFeedbackGain Kd, FeedForwardCurrent tau_ff) = 0;
        virtual bool setPosition(AngleRad position, VelocityRadPerSecond velocity, PositionFeedbackGain Kp, VelocityFeedbackGain Kd, FeedForwardCurrent tau_ff) = 0;

        // store potential constant values
        virtual bool configure(VelocityDegPerSecond velocity) = 0;
        virtual bool configure(VelocityRadPerSecond velocity) = 0;
        virtual bool configure(PositionFeedbackGain Kp) = 0;
        virtual bool configure(VelocityFeedbackGain Kd) = 0;
        virtual bool configure(FeedForwardCurrent tau_ff) = 0;

        // methods utilizing configured stored values for CAN commands
        virtual bool setPosition(AngleDeg position) = 0;
        virtual bool setPosition(AngleDeg position, VelocityDegPerSecond velocity) = 0;
        virtual bool setPosition(AngleRad position) = 0;
        virtual bool setPosition(AngleRad position, VelocityRadPerSecond velocity) = 0;
    };

} // namespace mini_cheetah_actuator_lib

#endif // MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_REQUEST_H