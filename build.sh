#!/bin/bash
set -euo pipefail

conan create packages/strong_types_lib/ -b missing -pr conan/profiles/default
conan create packages/can_interface_lib/ -b missing -pr conan/profiles/default
conan create packages/mini_cheetah_actuator_lib/ -b missing -pr conan/profiles/default
conan create packages/mini_cheetah_actuator_gui/ -b missing -pr conan/profiles/default
