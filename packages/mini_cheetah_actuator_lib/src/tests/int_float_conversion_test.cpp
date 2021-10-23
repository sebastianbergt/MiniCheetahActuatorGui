#include <catch2/catch.hpp>
#include <cmath>
#include <tuple>
#include <mini_cheetah_actuator_lib/actuator.h>

namespace sut = mini_cheetah_actuator_lib;

SCENARIO("Actuator: Helper functions for bit banging")
{
    GIVEN("values to convert")
    {
        const float X_VALUE{GENERATE(-1E6F, -1.5F, -0.0F, 0.0F, 1.0F, 1E6F)};

        using Range = std::tuple<float, float>;
        auto range = GENERATE(table<float, float>({
            Range{-10.0F, 10.0F},
            Range{-1.0F, 10.0F},
            Range{-10.0F, 0.0F},
            Range{-10.0F, 0.5F},
            Range{0.0F, 10.0F},
            Range{0.5F, 1.0F},
            Range{0.0F, 100.0F},
        }));
        const float X_MIN{std::get<0>(range)};
        const float X_MAX{std::get<1>(range)};

        const std::uint32_t BITS_USED{GENERATE(3U, 5U, 8U, 12U, 23U, 31U)};
        WHEN("conversions are called")
        {
            const auto integer_value = sut::toInt(X_VALUE, X_MIN, X_MAX, BITS_USED);
            const auto roundtrip_value = sut::toFloat(integer_value, X_MIN, X_MAX, BITS_USED);
            THEN("the roundtrip value is limited to boundaries or equals the input value when within boundaries")
            {
                // calculate maximum error
                const auto SPAN = X_MAX - X_MIN;
                const std::uint32_t MAXIMUM_INT_VALUE = (1 << BITS_USED) - 1;
                const float VALUE_OF_LSB = std::abs(SPAN / MAXIMUM_INT_VALUE);

                if (X_VALUE <= X_MIN)
                {
                    REQUIRE(Approx(roundtrip_value) == X_MIN);
                }
                else if (X_VALUE >= X_MAX)
                {
                    REQUIRE(Approx(roundtrip_value) == X_MAX);
                }
                else
                {
                    REQUIRE(Approx(roundtrip_value).margin(VALUE_OF_LSB) == X_VALUE);
                }
            }
        }
    }
}
