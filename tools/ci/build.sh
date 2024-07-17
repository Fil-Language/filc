#!/usr/bin/env bash

if [ -z "$1" ]; then
  platform=""
else
  platform="--platforms=//tools/bazel/platform:$1"
fi

bazel build -c opt $platform //:filc
