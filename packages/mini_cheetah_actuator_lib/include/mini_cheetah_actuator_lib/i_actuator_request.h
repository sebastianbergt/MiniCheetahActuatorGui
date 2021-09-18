#ifndef MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_REQUEST_H
#define MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_REQUEST_H

#include <strong_types_lib/strong_types.h>

namespace mini_cheetah_actuator_lib
{
    using AngleDeg = strong_types::StrongType<double, struct AngleDegTag>;
    using VelocityDegPerSecond = strong_types::StrongType<double, struct VelocityDegPerSecondTag>;
    using AngleRad = strong_types::StrongType<double, struct AngleRadTag>;
    using VelocityRadPerSecond = strong_types::StrongType<double, struct AngleRadTag>;
    using PositionFeedbackGain = strong_types::StrongType<double, struct PositionFeedbackGainTag>;
    using VelocityFeedbackGain = strong_types::StrongType<double, struct VelocityFeedbackGainTag>;
    using FeedForwardCurrent = strong_types::StrongType<double, struct FeedForwardCurrentTag>;

    class IActuatorRequest
    {
        virtual bool enable();
        virtual bool disable();
        // methods mapping directly to actuator CAN commands
        virtual bool setPosition(AngleDeg position, VelocityDegPerSecond velocity, PositionFeedbackGain Kp, VelocityFeedbackGain Kd, FeedForwardCurrent tau_ff);
        virtual bool setPosition(AngleRad position, VelocityRadPerSecond velocity, PositionFeedbackGain Kp, VelocityFeedbackGain Kd, FeedForwardCurrent tau_ff);

        // store potential constant values
        virtual bool configure(VelocityDegPerSecond velocity);
        virtual bool configure(VelocityRadPerSecond velocity);
        virtual bool configure(PositionFeedbackGain Kp);
        virtual bool configure(VelocityFeedbackGain Kd);
        virtual bool configure(FeedForwardCurrent tau_ff);

        // methods utilizing configured stored values for CAN commands
        virtual bool setPosition(AngleDeg position);
        virtual bool setPosition(AngleDeg position, VelocityDegPerSecond velocity);
        virtual bool setPosition(AngleRad position);
        virtual bool setPosition(AngleRad position, VelocityRadPerSecond velocity);
    };

} // namespace mini_cheetah_actuator_lib

#endif // MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_REQUEST_H