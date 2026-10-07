for i in {1..5}; do
    echo "Run $i"
    /usr/bin/time -f "%e seconds" \
        ./scripts/run_perf.sh scenes/benchmark.json benchmark.png
done