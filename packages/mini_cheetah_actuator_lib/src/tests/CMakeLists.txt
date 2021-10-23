add_executable(actuator_test actuator_test.cpp int_float_conversion_test.cpp
                             mock_can.h)

target_link_libraries(
  actuator_test PRIVATE Catch2::Catch2WithMain trompeloeil::trompeloeil
                        mini_cheetah_actuator_lib)
add_test(NAME actuator_test COMMAND actuator_test)
