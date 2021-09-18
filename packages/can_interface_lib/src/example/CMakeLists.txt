# find_package(can_interface_lib REQUIRED)

add_executable(can_example example.cpp)

target_link_libraries(can_example PRIVATE can_interface_lib)
