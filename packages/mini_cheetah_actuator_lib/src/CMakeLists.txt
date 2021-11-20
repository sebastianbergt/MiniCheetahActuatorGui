cmake_minimum_required(VERSION 3.17)

project(mini_cheetah_actuator_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# lib
add_library(
  mini_cheetah_actuator_lib
  include/mini_cheetah_actuator_lib/i_actuator.h
  include/mini_cheetah_actuator_lib/int_float_conversion.h
  include/mini_cheetah_actuator_lib/actuator.h
  src/actuator.cpp
  src/int_float_conversion.cpp)

target_include_directories(mini_cheetah_actuator_lib PUBLIC include)

find_package(strong_types_lib REQUIRED)
find_package(can_interface_lib REQUIRED)

target_link_libraries(
  mini_cheetah_actuator_lib PUBLIC strong_types_lib::strong_types_lib
                                   can_interface_lib::can_interface_lib)

# install
install(TARGETS mini_cheetah_actuator_lib)
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
