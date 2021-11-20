# MiniCheetahActuatorGui

![Screenshot](https://github.com/sebastianbergt/MiniCheetahActuatorGui/blob/main/images/MiniCheetahActuatorGui_v1.png?raw=true)

## Project Goal
Create a fully tested C++14 driver lib and imgui GUI to be used with [TMotor AK60-6 and other motors using the same driver](https://store.tmotor.com/goods.php?id=1138).

## Build instructions
The project is utilizing the excellent [conan package manager](https://conan.io/) and [cmake](https://cmake.org/).

[Trompeloeil](https://github.com/rollbear/trompeloeil/) is used for mocking and [Catch2](https://github.com/catchorg/Catch2) for unit tests.

```
./build.sh
```

## Checking coverage
Install gcovr through pip3

```
pip3 install gcovr
```

You should then be able to run the build with the coverage profile. Watch the output for a link to coverage.html file for each package.
```
./build_coverage.sh
```
## Status
:green_circle: Project is fully usable

## Todos
* Complete the socket can library -> done
* Create the actuator lib -> done and tested with AK60-6
* Enable clang tidy
* Enable all warnings -> done
* Create imgui application -> done

## Similar Projects and information
* Python class https://github.com/vyas-shubham/mini-cheetah-tmotor-python-can
* Documentation for MiniCheetahActuator https://docs.google.com/document/d/1dzNVzblz6mqB3eZVEMyi2MtSngALHdgpTaDJIW_BpS4/
* SocketCAN API https://www.beyondlogic.org/example-c-socketcan-code/
