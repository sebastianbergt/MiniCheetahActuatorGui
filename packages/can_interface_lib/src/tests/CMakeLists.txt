add_executable(socket_can_test socket_can_test.cpp) # i_can_test.cpp

target_link_libraries(
  socket_can_test PRIVATE Catch2::Catch2WithMain trompeloeil::trompeloeil
                          can_interface_lib)
add_test(NAME socket_can_test COMMAND socket_can_test)
