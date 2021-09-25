#include <mini_cheetah_actuator_lib/actuator_request.h>
#include <can_interface_lib/can.h>
#include <stdexcept>

namespace mini_cheetah_actuator_lib
{
    std::uint32_t toInt(float x, float x_min, float x_max, std::uint32_t bits)
    {
        const auto x_min_clamped = std::max(0.0F, x_min);
        const auto span = std::max(0.0F, x_max) - x_min_clamped;
        const auto x_clamped{std::min(std::max(std::max(0.0F, x), x_min), x_max)};

        const auto x_delta = x_clamped - x_min_clamped;

        const std::uint32_t maximum_int_value = (1 << bits) - 1;
        const float value_of_lsb = maximum_int_value / span;

        return x_delta * value_of_lsb;
    }

    float toFloat(std::uint32_t x, float x_min, float x_max, std::uint32_t bits)
    {
        const auto span = x_max - x_min;

        const std::uint32_t maximum_int_value = (1 << bits) - 1;
        const float value_of_lsb = span / maximum_int_value;

        return x * value_of_lsb + x_min;
    }

    ActuatorRequest::ActuatorRequest(std::string can_interface_name, MotorId motor_id, std::unique_ptr<can_interface_lib::ICan> can) : can_interface_name_{can_interface_name},
                                                                                                                                       motor_id_{motor_id}, can_{std::move(can)}
    {
        if (!can_)
        {
            can_ = can_interface_lib::makeCanInterface();
        }
    }

    bool ActuatorRequest::send_magic_number(std::uint8_t last_byte)
    {
        namespace cil = can_interface_lib;
        cil::CanFrame frame{};
        frame.id = cil::CanId{motor_id_.get()};
        frame.data = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, last_byte}};
        frame.bytes_used = cil::BytesUsed{8U};
        return can_->send(frame);
    }

    bool ActuatorRequest::enable()
    {
        if (!can_->isConnected())
        {
            if (!can_->connect(can_interface_name_))
            {
                return false;
            }
        }

        return send_magic_number(0xFC);
    }

    bool ActuatorRequest::disable()
    {
        return send_magic_number(0xFD);
    }

    bool ActuatorRequest::setCurrentPositionToZero()
    {
        return send_magic_number(0xFE);
    }

    bool ActuatorRequest::setPosition(const AngleDeg position, const VelocityDegPerSecond velocity, const PositionFeedbackGain Kp, const VelocityFeedbackGain Kd, const FeedForwardCurrent tau_ff)
    {
        (void)position;
        (void)velocity;
        (void)Kp;
        (void)Kd;
        (void)tau_ff;
        return true;
    }
    bool ActuatorRequest::setPosition(const AngleRad position, const VelocityRadPerSecond velocity, const PositionFeedbackGain Kp, const VelocityFeedbackGain Kd, const FeedForwardCurrent tau_ff)
    {
        (void)position;
        (void)velocity;
        (void)Kp;
        (void)Kd;
        (void)tau_ff;
        return true;
    }
    bool ActuatorRequest::configure(const VelocityDegPerSecond velocity)
    {
        (void)velocity;
        return true;
    }
    bool ActuatorRequest::configure(const VelocityRadPerSecond velocity)
    {
        velocity_rad_ = velocity;
        return true;
    }
    bool ActuatorRequest::configure(const PositionFeedbackGain Kp)
    {
        position_feedback_gain_ = Kp;
        return true;
    }
    bool ActuatorRequest::configure(const VelocityFeedbackGain Kd)
    {
        velocity_feedback_gain_ = Kd;
        return true;
    }
    bool ActuatorRequest::configure(const FeedForwardCurrent tau_ff)
    {
        feed_forward_current_ = tau_ff;
        return true;
    }
    bool ActuatorRequest::setPosition(const AngleDeg position)
    {
        (void)position;
        return true;
    }
    bool ActuatorRequest::setPosition(const AngleDeg position, const VelocityDegPerSecond velocity)
    {
        (void)position;
        (void)velocity;
        return true;
    }
    bool ActuatorRequest::setPosition(const AngleRad position)
    {
        (void)position;
        return true;
    }

    bool ActuatorRequest::setPosition(const AngleRad position, const VelocityRadPerSecond velocity)
    {
        (void)position;
        (void)velocity;
        return true;
    }
}