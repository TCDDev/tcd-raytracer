#!/usr/bin/env bash
set -euo pipefail

mkdir -p perf

perf record \
    -F 99 \
    -g \
    --call-graph dwarf \
    -o perf/perf.data \
    ./build/gcc-perf/ray_tracer

perf script -i perf/perf.data > perf/perf.out

~/dev/FlameGraph/stackcollapse-perf.pl \
    perf/perf.out > perf/perf.folded

~/dev/FlameGraph/flamegraph.pl \
    perf/perf.folded > perf/flamegraph.svg