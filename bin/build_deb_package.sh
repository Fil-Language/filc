#!/usr/bin/env bash

rm -rf build_deb_package
cmake -B build_deb_package -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build_deb_package || exit 1
cd build_deb_package || exit 1
cpack -G DEB
