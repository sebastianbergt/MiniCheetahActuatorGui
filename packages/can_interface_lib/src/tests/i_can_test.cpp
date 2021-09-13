#include <catch2/catch.hpp>
#include <can_interface_lib/i_can.h>

TEST_CASE("ICan")
{
    static_assert(std::is_abstract<can_interface_lib::ICan>::value,
                  "Interface class should be abstract");
    static_assert(std::has_virtual_destructor<can_interface_lib::ICan>::value,
                  "Interface class should have virtual destructor");
}