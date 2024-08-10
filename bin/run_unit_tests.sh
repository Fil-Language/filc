#!/usr/bin/env bash

rm -rf build_unit_tests
cmake -B build_unit_tests -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=On -DCODE_COVERAGE=On -G Ninja
cmake --build build_unit_tests --target unit-tests || exit 1
cd build_unit_tests || exit 1
ctest -L unit --output-on-failure
