#ifndef MINI_CHEETAH_ACTUATOR_LIB_INT_FLOAT_CONVERSION_H
#define MINI_CHEETAH_ACTUATOR_LIB_INT_FLOAT_CONVERSION_H

#include <cstdint>

namespace mini_cheetah_actuator_lib {

std::uint32_t toInt(float x, float x_min, float x_max, std::uint32_t bits);
float toFloat(std::uint32_t x, float x_min, float x_max, std::uint32_t bits);

} // namespace mini_cheetah_actuator_lib

#endif // MINI_CHEETAH_ACTUATOR_LIB_INT_FLOAT_CONVERSION_H