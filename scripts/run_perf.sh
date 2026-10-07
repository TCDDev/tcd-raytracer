#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build/gcc-perf"

if [ ! -x "$BUILD_DIR/ray_tracer" ]; then
cmake --preset gcc-perf
cmake --build --preset gcc-perf-build
fi

"$BUILD_DIR/ray_tracer" "$@"