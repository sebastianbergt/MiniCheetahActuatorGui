add_executable(strong_types_test strong_types_test.cpp)

target_link_libraries(strong_types_test PRIVATE strong_types_lib
                                                Catch2::Catch2WithMain)
add_test(NAME strong_types_test COMMAND strong_types_test)
