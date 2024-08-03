#!/usr/bin/env bash

FORMAT="clang-format -style=file -i"

$FORMAT main.cpp
find src -iname '*.h' -o -iname '*.cpp' -exec $FORMAT {} \;
find include -iname '*.h' -o -iname '*.cpp' -exec $FORMAT {} \;
find tests -iname '*.h' -o -iname '*.cpp' -exec $FORMAT {} \;
