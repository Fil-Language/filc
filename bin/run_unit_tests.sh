#!/usr/bin/env bash

set -euo pipefail

rm -rf build_unit_tests
cmake -B build_unit_tests -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=On -DCODE_COVERAGE=On -G Ninja
cmake --build build_unit_tests --target unit-tests
cd build_unit_tests
ctest -L unit --output-on-failure
