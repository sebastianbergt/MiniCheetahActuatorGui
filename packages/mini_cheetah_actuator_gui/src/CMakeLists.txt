cmake_minimum_required(VERSION 3.17)

project(mini_cheetah_actuator_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)

add_executable(
  mini_cheetah_actuator_gui
  src/main.cpp
  ${imgui_RES_DIRS}/bindings/imgui_impl_glfw.cpp
  ${imgui_RES_DIRS}/bindings/imgui_impl_glfw.h
  ${imgui_RES_DIRS}/bindings/imgui_impl_opengl3.cpp
  ${imgui_RES_DIRS}/bindings/imgui_impl_opengl3.h)

target_include_directories(mini_cheetah_actuator_gui
                           PRIVATE ${imgui_RES_DIRS}/bindings)

# find_package(mini_cheetah_actuator_lib REQUIRED)
find_package(can_interface_lib REQUIRED)

target_link_libraries(
  mini_cheetah_actuator_gui PRIVATE GLEW::GLEW glfw::glfw imgui::imgui
                                    can_interface_lib::can_interface_lib)
# mini_cheetah_actuator_lib::mini_cheetah_actuator_lib)

install(TARGETS mini_cheetah_actuator_gui)
