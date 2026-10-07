#!/usr/bin/env bash

set -euo pipefail

cmake --preset gcc-debug
cmake --build --preset gcc-build
./build/gcc-debug/ray_tracer_tests

cmake --preset clang-debug
cmake --build --preset clang-build
./build/clang-debug/ray_tracer_tests