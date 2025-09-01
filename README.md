# pthreads-sync-bench-c17

This repository contains simple benchmarks for exercises in concurrent data structures.

Build:

```
make
```

Run examples:

```
./timer_test
./counter_mutex <num_threads> <iters_per_thread>
./approx_counter <num_threads> <iters_per_thread> <threshold>
./list_hh <num_threads> <ops_per_thread>
./hash_single <num_threads> <ops_per_thread>
./hash_buckets <num_threads> <ops_per_thread>
```

Notes:
- Programs use `gettimeofday()` for timing measurements.
- Hash table size is large to reduce collisions; tune `TABLE_SIZE` in sources if desired.
- These are simple educational implementations; they are not production-ready.
