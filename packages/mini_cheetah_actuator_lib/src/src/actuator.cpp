#include <mini_cheetah_actuator_lib/actuator.h>
#include <can_interface_lib/can.h>
#include <cmath>
#include <stdexcept>

namespace mini_cheetah_actuator_lib
{
    std::uint32_t toInt(float x, float x_min, float x_max, std::uint32_t bits)
    {
        const auto span = x_max - x_min;
        const auto x_clamped{std::min(std::max(x, x_min), x_max)};

        const auto x_delta = x_clamped - x_min;

        const std::uint32_t maximum_int_value = (1 << bits) - 1;
        const float value_of_lsb = maximum_int_value / span;

        return x_delta * value_of_lsb;
    }

    float toFloat(std::uint32_t x, float x_min, float x_max, std::uint32_t bits)
    {
        const auto span = x_max - x_min;

        const std::uint32_t maximum_int_value = (1 << bits) - 1;
        const float inverse_value_of_lsb = span / maximum_int_value;

        return x * inverse_value_of_lsb + x_min;
    }

    Actuator::Actuator(std::string can_interface_name, MotorId motor_id, std::unique_ptr<can_interface_lib::ICan> can) : can_interface_name_{can_interface_name},
                                                                                                                         motor_id_{motor_id}, can_{std::move(can)}
    {
        if (!can_)
        {
            can_ = can_interface_lib::makeCanInterface();
        }
    }

    Actuator::~Actuator()
    {
        if (can_)
        {
            can_->disconnect();
        }
    }

    bool Actuator::send_magic_number(std::uint8_t last_byte)
    {
        namespace cil = can_interface_lib;
        cil::CanFrame frame{};
        frame.id = cil::CanId{motor_id_.get()};
        frame.data = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, last_byte}};
        frame.bytes_used = cil::BytesUsed{8U};
        return can_->send(frame);
    }

    bool Actuator::enable()
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

    bool Actuator::disable()
    {
        return send_magic_number(0xFD);
    }

    bool Actuator::setCurrentPositionToZero()
    {
        return send_magic_number(0xFE);
    }

    can_interface_lib::CanFrame Actuator::serialize(const AngleRad position, const VelocityRadPerSecond velocity, const PositionFeedbackGain position_feedback_gain, const VelocityFeedbackGain velocity_feedback_gain, const FeedForwardCurrentAmpere feed_forward_current)
    {
        can_interface_lib::CanFrame frame{};
        frame.id = can_interface_lib::CanId{motor_id_.get()};

        const std::uint32_t position_int{toInt(position.get(), POSITION_MIN, POSITION_MAX, POSITION_BITS)};
        const std::uint32_t velocity_int{toInt(velocity.get(), VELOCITY_RAD_PER_SECOND_MIN, VELOCITY_RAD_PER_SECOND_MAX, VELOCITY_RAD_PER_SECOND_BITS)};
        const std::uint32_t position_feedback_gain_int{toInt(position_feedback_gain.get(), POSITION_FEEDBACK_GAIN_MIN, POSITION_FEEDBACK_GAIN_MAX, POSITION_FEEDBACK_GAIN_BITS)};
        const std::uint32_t velocity_feedback_gain_int{toInt(velocity_feedback_gain.get(), VELOCITY_FEEDBACK_GAIN_MIN, VELOCITY_FEEDBACK_GAIN_MAX, VELOCITY_FEEDBACK_GAIN_BITS)};
        const std::uint32_t feed_forward_current_int{toInt(feed_forward_current.get(), FEED_FORWARD_CURRENT_MIN, FEED_FORWARD_CURRENT_MAX, FEED_FORWARD_CURRENT_BITS)};

        frame.data[0] = 0xFF & (position_int >> 8);
        frame.data[1] = 0xFF & position_int;

        frame.data[2] = 0xFF & (velocity_int >> 4);
        frame.data[3] = 0xF0 & (velocity_int << 4);

        frame.data[3] |= 0x0F & (position_feedback_gain_int >> 8);
        frame.data[4] = 0xFF & position_feedback_gain_int;

        frame.data[5] = 0xFF & (velocity_feedback_gain_int >> 4);
        frame.data[6] = 0xF0 & (velocity_feedback_gain_int << 4);

        frame.data[6] |= 0x0F & (feed_forward_current_int >> 8);
        frame.data[7] = 0xFF & feed_forward_current_int;

        frame.bytes_used = can_interface_lib::BytesUsed{8UL};

        return frame;
    }

    constexpr float deg_to_rad(const float &in)
    {
        return in * (M_PI / 180.0F);
    }

    bool Actuator::setPosition(const AngleDeg position, const VelocityDegPerSecond velocity, const PositionFeedbackGain position_feedback_gain, const VelocityFeedbackGain velocity_feedback_gain, const FeedForwardCurrentAmpere feed_forward_current)
    {
        const AngleRad position_in_rad{deg_to_rad(position.get())};
        const VelocityRadPerSecond velocity_in_rad{deg_to_rad(velocity.get())};
        const auto frame = serialize(position_in_rad, velocity_in_rad, position_feedback_gain, velocity_feedback_gain, feed_forward_current);
        return can_->send(frame);
    }

    bool Actuator::setPosition(const AngleRad position, const VelocityRadPerSecond velocity, const PositionFeedbackGain position_feedback_gain, const VelocityFeedbackGain velocity_feedback_gain, const FeedForwardCurrentAmpere feed_forward_current)
    {
        const auto frame = serialize(position, velocity, position_feedback_gain, velocity_feedback_gain, feed_forward_current);
        return can_->send(frame);
    }

    void Actuator::configure(const VelocityDegPerSecond velocity)
    {
        velocity_rad_ = VelocityRadPerSecond{deg_to_rad(velocity.get())};
    }

    void Actuator::configure(const VelocityRadPerSecond velocity)
    {
        velocity_rad_ = velocity;
    }

    void Actuator::configure(const PositionFeedbackGain position_feedback_gain)
    {
        position_feedback_gain_ = position_feedback_gain;
    }

    void Actuator::configure(const VelocityFeedbackGain velocity_feedback_gain)
    {
        velocity_feedback_gain_ = velocity_feedback_gain;
    }

    void Actuator::configure(const FeedForwardCurrentAmpere feed_forward_current)
    {
        feed_forward_current_ = feed_forward_current;
    }

    bool Actuator::setPosition(const AngleDeg position)
    {
        const AngleRad position_in_rad{deg_to_rad(position.get())};
        const auto frame = serialize(position_in_rad, velocity_rad_, position_feedback_gain_, velocity_feedback_gain_, feed_forward_current_);
        return can_->send(frame);
    }

    bool Actuator::setPosition(const AngleDeg position, const VelocityDegPerSecond velocity)
    {
        const AngleRad position_in_rad{deg_to_rad(position.get())};
        const VelocityRadPerSecond velocity_in_rad{deg_to_rad(velocity.get())};
        const auto frame = serialize(position_in_rad, velocity_in_rad, position_feedback_gain_, velocity_feedback_gain_, feed_forward_current_);
        return can_->send(frame);
    }

    bool Actuator::setPosition(const AngleRad position)
    {
        const auto frame = serialize(position, velocity_rad_, position_feedback_gain_, velocity_feedback_gain_, feed_forward_current_);
        return can_->send(frame);
    }

    bool Actuator::setPosition(const AngleRad position, const VelocityRadPerSecond velocity)
    {
        const auto frame = serialize(position, velocity, position_feedback_gain_, velocity_feedback_gain_, feed_forward_current_);
        return can_->send(frame);
    }

    bool Actuator::getStatus(Status &status)
    {
        can_interface_lib::CanFrame frame{};
        if (!can_->receive(frame))
        {
            return false;
        }

        const auto motor_id = frame.data[0];
        if (motor_id != motor_id_.get())
        {
            return false;
        }

        const auto positon_int = (frame.data[1] << 8U) | frame.data[2];
        const auto velocity_int = (frame.data[3] << 4U) | ((0xF0 & frame.data[4]) >> 4U);
        const auto current_int = ((0x0F & frame.data[4]) << 8) | frame.data[5];

        status.motor_id = MotorId{motor_id};
        status.positon = AngleRad{toFloat(positon_int, POSITION_MIN, POSITION_MAX, POSITION_BITS)};
        status.velocity = VelocityRadPerSecond{toFloat(velocity_int, VELOCITY_RAD_PER_SECOND_MIN, VELOCITY_RAD_PER_SECOND_MAX, VELOCITY_RAD_PER_SECOND_BITS)};
        status.current = CurrentAmpere{toFloat(current_int, POSITION_MIN, POSITION_MAX, POSITION_BITS)};

        return true;
    }
}