#!/usr/bin/env bash

set -e
set -euo pipefail

cmake --preset gcc-debug
cmake --build --preset gcc-build
./build/gcc-debug/ray_tracer

cmake --preset clang-debug
cmake --build --preset clang-build
./build/clang-debug/ray_tracer