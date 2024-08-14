#!/usr/bin/env bash

rm -rf build_rpm_package
cmake -B build_rpm_package -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build_rpm_package || exit 1
cd build_rpm_package || exit 1
cpack -G RPM
