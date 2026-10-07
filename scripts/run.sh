#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build/gcc-debug"

if [ ! -x "$BUILD_DIR/ray_tracer" ]; then
cmake --preset gcc-debug
cmake --build --preset gcc-build
fi

"$BUILD_DIR/ray_tracer" "$@"