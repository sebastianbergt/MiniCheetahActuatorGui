#include <catch2/catch.hpp>
#include <strong_types_lib/strong_types.h>

TEST_CASE("Strong Types")
{
    constexpr float DEFAULT_VALUE{1.0F};

    using Degrees = strong_types::StrongType<float, struct MetersTag>;
    Degrees strong_typed_angle{DEFAULT_VALUE};

    const auto angle_copy = strong_typed_angle.get();
    const auto &angle_ref = strong_typed_angle.get();

    CHECK(angle_copy == Approx(DEFAULT_VALUE));
    CHECK(angle_ref == Approx(DEFAULT_VALUE));
}