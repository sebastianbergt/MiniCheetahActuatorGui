#include <algorithm>
#include <cmath>
#include <mini_cheetah_actuator_lib/int_float_conversion.h>

namespace mini_cheetah_actuator_lib {

std::uint32_t toInt(float x, float x_min, float x_max, std::uint32_t bits) {
  const auto span = x_max - x_min;
  const auto x_clamped{std::min(std::max(x, x_min), x_max)};

  const auto x_delta = x_clamped - x_min;

  const std::uint32_t maximum_int_value = (1 << bits) - 1;
  const float value_of_lsb = maximum_int_value / span;

  return x_delta * value_of_lsb;
}

float toFloat(std::uint32_t x, float x_min, float x_max, std::uint32_t bits) {
  const auto span = x_max - x_min;

  const std::uint32_t maximum_int_value = (1 << bits) - 1;
  const float inverse_value_of_lsb = span / maximum_int_value;

  return x * inverse_value_of_lsb + x_min;
}
} // namespace mini_cheetah_actuator_lib
