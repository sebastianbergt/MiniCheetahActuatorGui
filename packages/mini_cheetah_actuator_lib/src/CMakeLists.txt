cmake_minimum_required(VERSION 3.1)

project(mini_cheetah_actuator_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# lib
find_package(can_interface_lib REQUIRED)

add_library(
  mini_cheetah_actuator_lib
  include/mini_cheetah_actuator_lib/i_actuator_request.h
  src/actuator_request.cpp)
target_include_directories(mini_cheetah_actuator_lib PUBLIC include)
target_link_libraries(mini_cheetah_actuator_lib PRIVATE can_interface_lib)

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
  find_package(Catch2 REQUIRED)
  find_package(trompeloeil REQUIRED)
  add_subdirectory(tests)
endif()
