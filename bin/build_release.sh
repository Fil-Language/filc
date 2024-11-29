#!/usr/bin/env bash

set -euo pipefail

rm -rf build_release
cmake -B build_release -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build_release
cp "$(cat build_release/filc.path)" .
