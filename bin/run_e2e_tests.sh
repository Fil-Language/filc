#!/usr/bin/env bash

rm -rf build_e2e_tests
cmake -B build_e2e_tests -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=On -G Ninja
cmake --build build_e2e_tests --target e2e-tests || exit 1
cd build_e2e_tests || exit 1
ctest -L e2e --output-on-failure
