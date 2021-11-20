#ifndef MINI_CHEETAH_ACTUATOR_LIB_ACTUATOR_H
#define MINI_CHEETAH_ACTUATOR_LIB_ACTUATOR_H

#include <algorithm>
#include <can_interface_lib/can.h>
#include <cstdint>
#include <memory>
#include <mini_cheetah_actuator_lib/i_actuator.h>
#include <string>

namespace mini_cheetah_actuator_lib {

class Actuator : public IActuator {
public:
  Actuator(std::string can_interface_name, MotorId motor_id,
           can_interface_lib::ICan &can);
  ~Actuator();
  Actuator(Actuator const &other) = delete;
  Actuator(Actuator &&that) = delete;
  Actuator &operator=(Actuator const &other) & = delete;
  Actuator &operator=(Actuator &&that) & = delete;

  bool enable();
  bool disable();
  bool setCurrentPositionToZero();
  // methods mapping directly to actuator CAN commands
  bool setPosition(AngleDeg position, VelocityDegPerSecond velocity,
                   PositionFeedbackGain position_feedback_gain,
                   VelocityFeedbackGain velocity_feedback_gain,
                   FeedForwardCurrentAmpere feed_forward_current) override;
  bool setPosition(AngleRad position, VelocityRadPerSecond velocity,
                   PositionFeedbackGain position_feedback_gain,
                   VelocityFeedbackGain velocity_feedback_gain,
                   FeedForwardCurrentAmpere feed_forward_current) override;

  // store potential constant values
  void configure(VelocityDegPerSecond velocity) override;
  void configure(VelocityRadPerSecond velocity) override;
  void configure(PositionFeedbackGain position_feedback_gain) override;
  void configure(VelocityFeedbackGain velocity_feedback_gain) override;
  void configure(FeedForwardCurrentAmpere feed_forward_current) override;

  // methods utilizing configured stored values for CAN commands
  bool setPosition(AngleDeg position) override;
  bool setPosition(AngleDeg position, VelocityDegPerSecond velocity) override;
  bool setPosition(AngleRad position) override;
  bool setPosition(AngleRad position, VelocityRadPerSecond velocity) override;

  // reading from Motor
  bool getStatus(Status &status) override;

private:
  bool send_magic_number(std::uint8_t last_byte);
  can_interface_lib::CanFrame
  serialize(const AngleRad position, const VelocityRadPerSecond velocity,
            const PositionFeedbackGain position_feedback_gain,
            const VelocityFeedbackGain velocity_feedback_gain,
            const FeedForwardCurrentAmpere feed_forward_current);

  std::string can_interface_name_;
  MotorId motor_id_;
  can_interface_lib::ICan &can_;

  VelocityRadPerSecond velocity_rad_{0.0F};
  PositionFeedbackGain position_feedback_gain_{0.0F};
  VelocityFeedbackGain velocity_feedback_gain_{0.0F};
  FeedForwardCurrentAmpere feed_forward_current_{0.0F};
};
} // namespace mini_cheetah_actuator_lib

#endif // MINI_CHEETAH_ACTUATOR_LIB_ACTUATOR_H