# MiniCheetahActuatorGui

## Project Goal
Create a fully tested C++ driver lib and imgui GUI

## Build instructions
The project is utilizing the excellent [conan package manager](https://conan.io/) and [cmake](https://cmake.org/).
[Trompeloeil](https://github.com/rollbear/trompeloeil/) is used for mocking and [Catch2](https://github.com/catchorg/Catch2) for unit tests.

Currently you need to build the packages individually.

'conan create packages/strong_types_lib/ -b missing'
'conan create packages/can_interface_lib/ -b missing'

# Status
:stop_sign: Project is not usable yet

## Todos
* Complete the library driver
* Create imgui application

## Similar Projects and information
* Python class https://github.com/vyas-shubham/mini-cheetah-tmotor-python-can
* Documentation for MiniCheetahActuator https://docs.google.com/document/d/1dzNVzblz6mqB3eZVEMyi2MtSngALHdgpTaDJIW_BpS4/
* SocketCAN API https://www.beyondlogic.org/example-c-socketcan-code/
