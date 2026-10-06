#!/usr/bin/env bash

set -e
set -euo pipefail

cmake --preset gcc-perf
cmake --build --preset gcc-perf-build

cmake --preset clang-perf
cmake --build --preset clang-perf-build