#!/usr/bin/env bash

set -euo pipefail

rm -rf build_deb_package
cmake -B build_deb_package -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build_deb_package
cd build_deb_package
cpack -G DEB
