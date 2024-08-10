#!/usr/bin/env bash

rm -rf build_release
cmake -B build_release -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build_release || exit 1
cp "$(cat build_release/filc.path)" .
