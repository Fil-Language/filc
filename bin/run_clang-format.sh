#!/usr/bin/env bash

set -euo pipefail

FORMAT="clang-format -style=file -i"

$FORMAT main.cpp
find src -iname '*.h' -o -iname '*.cpp' -exec $FORMAT {} \;
find include -iname '*.h' -o -iname '*.cpp' -exec $FORMAT {} \;
find tests -iname '*.h' -o -iname '*.cpp' -exec $FORMAT {} \;
