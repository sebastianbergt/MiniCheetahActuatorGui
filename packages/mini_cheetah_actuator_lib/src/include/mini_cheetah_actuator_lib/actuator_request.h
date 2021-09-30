#ifndef MINI_CHEETAH_ACTUATOR_LIB_ACTUATOR_REQUEST_H
#define MINI_CHEETAH_ACTUATOR_LIB_ACTUATOR_REQUEST_H

#include <mini_cheetah_actuator_lib/i_actuator_request.h>
#include <can_interface_lib/can.h>
#include <algorithm>
#include <memory>
#include <string>

namespace mini_cheetah_actuator_lib
{
    std::uint32_t toInt(float x, float x_min, float x_max, std::uint32_t bits);
    float toFloat(std::uint32_t x, float x_min, float x_max, std::uint32_t bits);

    class ActuatorRequest : public IActuatorRequest
    {
    public:
        ActuatorRequest(std::string can_interface_name, MotorId motor_id, std::unique_ptr<can_interface_lib::ICan> can = nullptr);

        bool enable();
        bool disable();
        bool setCurrentPositionToZero();
        // methods mapping directly to actuator CAN commands
        bool setPosition(AngleDeg position, VelocityDegPerSecond velocity, PositionFeedbackGain Kp, VelocityFeedbackGain Kd, FeedForwardCurrent tau_ff) override;
        bool setPosition(AngleRad position, VelocityRadPerSecond velocity, PositionFeedbackGain Kp, VelocityFeedbackGain Kd, FeedForwardCurrent tau_ff) override;

        // store potential constant values
        bool configure(VelocityDegPerSecond velocity) override;
        bool configure(VelocityRadPerSecond velocity) override;
        bool configure(PositionFeedbackGain Kp) override;
        bool configure(VelocityFeedbackGain Kd) override;
        bool configure(FeedForwardCurrent tau_ff) override;

        // methods utilizing configured stored values for CAN commands
        bool setPosition(AngleDeg position) override;
        bool setPosition(AngleDeg position, VelocityDegPerSecond velocity) override;
        bool setPosition(AngleRad position) override;
        bool setPosition(AngleRad position, VelocityRadPerSecond velocity) override;

    private:
        bool send_magic_number(std::uint8_t last_byte);
        can_interface_lib::CanFrame serialize(const AngleDeg position, const VelocityDegPerSecond velocity, const PositionFeedbackGain Kp, const VelocityFeedbackGain Kd, const FeedForwardCurrent tau_ff);

        std::string can_interface_name_;
        MotorId motor_id_;
        std::unique_ptr<can_interface_lib::ICan> can_;

        VelocityDegPerSecond velocity_deg_{0.0F};
        VelocityRadPerSecond velocity_rad_{0.0F};
        PositionFeedbackGain position_feedback_gain_{0.0F};
        VelocityFeedbackGain velocity_feedback_gain_{0.0F};
        FeedForwardCurrent feed_forward_current_{
            0.0F};
    };
} // namespace mini_cheetah_actuator_lib

#endif // MINI_CHEETAH_ACTUATOR_LIB_ACTUATOR_REQUEST_H