add_executable(actuator_request_test actuator_request_test.cpp mock_can.h)

target_link_libraries(
  actuator_request_test PRIVATE Catch2::Catch2WithMain trompeloeil::trompeloeil
                                mini_cheetah_actuator_lib)
add_test(NAME actuator_request_test COMMAND actuator_request_test)
