#!/usr/bin/env bash

lcov --directory . --capture --output-file coverage.info

# Remove external files and dependencies
lcov --remove coverage.info '/nix/store/*' --output-file coverage.info
lcov --remove coverage.info '*/antlr4_runtime/*' '*/antlr4cpp_generated_src/*' --output-file coverage.info
lcov --remove coverage.info '*/cxxopts.hpp' --output-file coverage.info

# Display result
lcov --list coverage.info
