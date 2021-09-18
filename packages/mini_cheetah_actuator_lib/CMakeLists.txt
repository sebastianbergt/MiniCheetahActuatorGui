cmake_minimum_required(VERSION 3.1)

project(mini_cheetah_actuator_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# lib

add_library(
  mini_cheetah_actuator_lib INTERFACE
  include/mini_cheetah_actuator_lib/i_actuator_request.h
  src/actuator_request.cpp)
target_include_directories(mini_cheetah_actuator_lib INTERFACE include)

# tests

if(WITH_TESTS)
  include(CTest)
  find_package(Catch2 REQUIRED)
  add_subdirectory(tests)
endif()
