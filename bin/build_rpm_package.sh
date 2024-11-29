#!/usr/bin/env bash

set -euo pipefail

rm -rf build_rpm_package
cmake -B build_rpm_package -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build_rpm_package
echo "* $(date +"%a %b %d %Y") filc developers - $(cat VERSION)
- Package from git sha $(git show-ref --head -s HEAD)" > build_rpm_package/changelog
cd build_rpm_package
cpack -G RPM
