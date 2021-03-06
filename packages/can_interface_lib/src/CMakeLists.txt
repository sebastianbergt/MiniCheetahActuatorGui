cmake_minimum_required(VERSION 3.17)

project(can_interface_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# lib
add_library(
  can_interface_lib
  include/can_interface_lib/can.h include/can_interface_lib/i_can.h
  include/can_interface_lib/can_frame.h include/can_interface_lib/socket_can.h
  src/socket_can.cpp src/can.cpp)
target_include_directories(can_interface_lib PUBLIC include)

find_package(strong_types_lib REQUIRED)
target_link_libraries(can_interface_lib
                      PUBLIC strong_types_lib::strong_types_lib)

install(TARGETS can_interface_lib)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.h*")

# tests
if(WITH_TESTS)
  include(CTest)
  list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
  find_package(Catch2 REQUIRED)
  find_package(trompeloeil REQUIRED)
  add_subdirectory(tests)
endif()

# build example
add_subdirectory(example)
