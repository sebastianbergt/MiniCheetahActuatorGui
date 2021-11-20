#!/bin/bash
set -euo pipefail

conan create packages/strong_types_lib/ -b missing -pr conan/profiles/coverage
conan create packages/can_interface_lib/ -b missing -pr conan/profiles/coverage
conan create packages/mini_cheetah_actuator_lib/ -b missing -pr conan/profiles/coverage
