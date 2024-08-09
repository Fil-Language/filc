#!/usr/bin/env bash

cmake -B build_e2e_tests -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=On -G Ninja
cmake --build build_e2e_tests --target e2e-tests
cd build_e2e_tests || return 1
ctest -L e2e --output-on-failure
