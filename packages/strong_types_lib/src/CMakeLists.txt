cmake_minimum_required(VERSION 3.1)

project(strong_types_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# lib
add_library(strong_types_lib INTERFACE)
target_include_directories(strong_types_lib INTERFACE include)

install(TARGETS strong_types_lib)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.h*")

# tests
if(WITH_TESTS)
  include(CTest)
  find_package(Catch2 REQUIRED)
  add_subdirectory(tests)
endif()
