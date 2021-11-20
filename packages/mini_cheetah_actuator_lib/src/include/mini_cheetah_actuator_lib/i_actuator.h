#ifndef MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_H
#define MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_H

#include <cstdint>
#include <memory>
#include <string>
#include <strong_types_lib/strong_types.h>

namespace mini_cheetah_actuator_lib {
constexpr float POSITION_MIN{-12.5F};
constexpr float POSITION_MAX{12.5F};
constexpr std::uint32_t POSITION_BITS{16U};

constexpr float VELOCITY_RAD_PER_SECOND_MIN{-30.0F};
constexpr float VELOCITY_RAD_PER_SECOND_MAX{30.0F};
constexpr std::uint32_t VELOCITY_RAD_PER_SECOND_BITS{12U};

constexpr float POSITION_FEEDBACK_GAIN_MIN{0.0F};
constexpr float POSITION_FEEDBACK_GAIN_MAX{500.0F};
constexpr std::uint32_t POSITION_FEEDBACK_GAIN_BITS{12U};

constexpr float VELOCITY_FEEDBACK_GAIN_MIN{0.0F};
constexpr float VELOCITY_FEEDBACK_GAIN_MAX{5.0F};
constexpr std::uint32_t VELOCITY_FEEDBACK_GAIN_BITS{12U};

constexpr float FEED_FORWARD_CURRENT_MIN{-18.0F};
constexpr float FEED_FORWARD_CURRENT_MAX{18.0F};
constexpr std::uint32_t FEED_FORWARD_CURRENT_BITS{12U};

using AngleDeg = strong_types::StrongType<float, struct AngleDegTag>;
using VelocityDegPerSecond =
    strong_types::StrongType<float, struct VelocityDegPerSecondTag>;
using AngleRad = strong_types::StrongType<float, struct AngleRadTag>;
using VelocityRadPerSecond =
    strong_types::StrongType<float, struct VelocityRadPerSecondTag>;
using PositionFeedbackGain =
    strong_types::StrongType<float, struct PositionFeedbackGainTag>;
using VelocityFeedbackGain =
    strong_types::StrongType<float, struct VelocityFeedbackGainTag>;
using FeedForwardCurrentAmpere =
    strong_types::StrongType<float, struct FeedForwardCurrentAmpereTag>;
using CurrentAmpere = strong_types::StrongType<float, struct CurrentAmpereTag>;
using MotorId = strong_types::StrongType<std::uint8_t, struct MotorIdTag>;

struct Status {
  MotorId motor_id;
  AngleRad positon;
  VelocityRadPerSecond velocity;
  CurrentAmpere current;
};

class IActuator {
public:
  virtual bool enable() = 0;
  virtual bool disable() = 0;
  virtual bool setCurrentPositionToZero() = 0;
  // methods mapping directly to actuator CAN commands
  virtual bool setPosition(AngleDeg position, VelocityDegPerSecond velocity,
                           PositionFeedbackGain position_feedback_gain,
                           VelocityFeedbackGain velocity_feedback_gain,
                           FeedForwardCurrentAmpere feed_forward_current) = 0;
  virtual bool setPosition(AngleRad position, VelocityRadPerSecond velocity,
                           PositionFeedbackGain position_feedback_gain,
                           VelocityFeedbackGain velocity_feedback_gain,
                           FeedForwardCurrentAmpere feed_forward_current) = 0;

  // store potential constant values
  virtual void configure(VelocityDegPerSecond velocity) = 0;
  virtual void configure(VelocityRadPerSecond velocity) = 0;
  virtual void configure(PositionFeedbackGain position_feedback_gain) = 0;
  virtual void configure(VelocityFeedbackGain velocity_feedback_gain) = 0;
  virtual void configure(FeedForwardCurrentAmpere feed_forward_current) = 0;

  // methods utilizing configured stored values for CAN commands
  virtual bool setPosition(AngleDeg position) = 0;
  virtual bool setPosition(AngleDeg position,
                           VelocityDegPerSecond velocity) = 0;
  virtual bool setPosition(AngleRad position) = 0;
  virtual bool setPosition(AngleRad position,
                           VelocityRadPerSecond velocity) = 0;

  // reading from Motor values
  virtual bool getStatus(Status &status) = 0;
};

} // namespace mini_cheetah_actuator_lib

#endif // MINI_CHEETAH_ACTUATOR_LIB_I_ACTUATOR_H