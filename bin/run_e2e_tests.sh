#!/usr/bin/env bash

set -euo pipefail

rm -rf build_e2e_tests
cmake -B build_e2e_tests -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=On -G Ninja
cmake --build build_e2e_tests --target e2e-tests
cd build_e2e_tests
ctest -L e2e --output-on-failure
