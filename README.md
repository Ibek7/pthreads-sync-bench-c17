# pthreads-sync-bench-c17

Concurrent data structures homework assignment: implementations of various synchronization strategies and their performance characteristics.

## Project Overview

This project implements four key concurrent programming exercises using POSIX threads (pthreads):

1. **Timer Accuracy Measurement** - Measure gettimeofday() resolution
2. **Simple Concurrent Counter** - Basic mutex-protected counter with thread scaling analysis
3. **Approximate Counter** - Optimized counter using per-thread batching
4. **Hand-over-Hand Linked List** - Lock-coupling traversal strategy

## Quick Start

### Build
```bash
make
```

### Run Programs
```bash
./timer_test                                    # Measure timer resolution
./counter_mutex <threads> <iterations>          # Simple mutex counter
./approx_counter <threads> <iterations> <threshold>  # Approximate counter
./list_single <threads> <operations>            # Single-lock linked list
./list_hh <threads> <operations>                # Hand-over-hand linked list
./hash_single <threads> <operations>            # Single-lock hash table (bonus)
./hash_buckets <threads> <operations>           # Per-bucket hash table (bonus)
```

### Run Full Benchmark Suite
```bash
python3 bench_runner.py --threads 1 2 4 8 --ops 50000 --trials 3 --out results.csv
python3 summarize_results.py --in results.csv --out summary.csv
python3 plot_results.py --in results.csv --out results.png
```

## Project Structure

```
pthreads-sync-bench-c17/
├── Source Files
│   ├── timer_test.c              # Requirement 1: Timer accuracy
│   ├── counter_mutex.c           # Requirement 2: Simple counter
│   ├── approx_counter.c          # Requirement 3: Approximate counter
│   ├── list_single.c             # Requirement 4: Single-lock list
│   ├── list_hh.c                 # Requirement 4: Hand-over-hand list
│   ├── hash_single.c             # Bonus: Single-lock hash table
│   └── hash_buckets.c            # Bonus: Per-bucket hash table
├── Build System
│   └── Makefile                  # Compilation targets
├── Testing Tools
│   ├── bench_runner.py           # Parameterized benchmark harness
│   ├── summarize_results.py      # Statistical analysis
│   ├── plot_results.py           # Performance visualization
│   └── requirements.txt          # Python dependencies
├── Results
│   ├── results_full.csv          # Benchmark measurements (60+ data points)
│   ├── results_full_summary.csv  # Statistical summaries
│   └── results_full.png          # Performance graphs
├── Documentation
│   ├── README.md                 # This file
│   ├── report.txt                # Initial results and analysis
│   ├── COMPREHENSIVE_TEST_REPORT.md  # Full requirement verification
│   ├── VERIFICATION_CHECKLIST.md     # Implementation checklist
│   ├── PROJECT_SUMMARY.md            # Executive summary
│   └── LICENSE                       # MIT License
```

## Key Results

### Requirement 1: Timer Accuracy
- **Resolution**: 1 microsecond (smallest measurable interval)
- **Zero intervals**: 98.26% of 1,000,000 trials
- **Accuracy**: ±26% on 1ms sleep measurement

### Requirement 2: Concurrent Counter Scaling
- **System**: 8 CPU cores (Apple Silicon M-series)
- **Scaling degradation**: 1.0x → 3.7x → 13.9x → 27.3x (for 1/2/4/8 threads)
- **CPU Impact**: YES - Linear relationship with thread count

### Requirement 3: Approximate Counter
- **Speedup vs simple counter**: 46-89x faster
- **Optimal threshold**: 500-5000 operations per batch
- **Result**: Per-thread batching dramatically reduces lock contention

### Requirement 4: Hand-over-Hand List
- **Implementation**: Per-node locks with lock coupling
- **Comparison**: Better scaling at high thread counts (13.5x vs 27.3x at 8 threads)
- **Trade-off**: Higher per-operation cost but better parallelism

## Documentation Guide

Start with these files for different levels of detail:

| File | Purpose | Audience |
|------|---------|----------|
| **README.md** | Overview and quick start | Everyone |
| **PROJECT_SUMMARY.md** | Executive summary with key results | Instructors/Quick review |
| **COMPREHENSIVE_TEST_REPORT.md** | Full requirement verification | Detailed analysis |
| **VERIFICATION_CHECKLIST.md** | Implementation verification | Code review |
| **report.txt** | Performance results and analysis | Technical details |

## System Requirements

- macOS or Linux with POSIX threads support
- C11 compiler (gcc or clang with -pthread support)
- Python 3.6+ (for benchmarking tools, optional)
- matplotlib (optional, for plotting results)

## Building and Running

### Compile
```bash
make clean
make -j4
```

### Run Individual Tests
```bash
# Timer accuracy
./timer_test

# Mutex counter (4 threads, 100k increments each)
./counter_mutex 4 100000

# Approximate counter (4 threads, 100k iters, threshold 1000)
./approx_counter 4 100000 1000

# Linked list tests
./list_single 4 50000
./list_hh 4 50000
```

### Run Full Benchmark Suite
```bash
# Generate results
python3 bench_runner.py --threads 1 2 4 8 --ops 50000 --trials 3 --out results.csv

# Analyze results
python3 summarize_results.py --in results.csv --out summary.csv

# Visualize
python3 plot_results.py --in results.csv --out results.png
```

## Implementation Details

### Timer (timer_test.c)
- Runs 1,000,000 consecutive gettimeofday() calls
- Measures smallest non-zero time interval
- Validates accuracy with select() sleep sanity check

### Simple Counter (counter_mutex.c)
- Uses single pthread_mutex_t for synchronization
- Tests scaling from 1 to 8 threads
- Verifies correctness of final counter value

### Approximate Counter (approx_counter.c)
- Per-thread local counters (no synchronization)
- Atomic global counter for periodic flushing
- Configurable threshold for batching

### Single-Lock List (list_single.c)
- One global mutex protecting entire list
- Maintains sorted order
- Supports insert and contains operations

### Hand-over-Hand List (list_hh.c)
- Per-node pthread_mutex_t locks
- Lock coupling: acquire next before releasing previous
- Allows concurrent traversals of different list regions

## Performance Insights

1. **Lock Contention**: Simple mutex shows near-linear scaling degradation
2. **Batching Optimization**: 46-89x speedup through reduced synchronization
3. **Fine-Grained Locking**: Per-bucket locks 5-10x better than global lock
4. **Lock Coupling**: Better scaling at high thread counts despite higher overhead

## Code Quality

- ✅ Clean compilation: no errors or warnings (-Wall -Wextra)
- ✅ Proper thread synchronization (no race conditions or deadlocks)
- ✅ Accurate performance measurement with gettimeofday()
- ✅ Correct handling of thread creation and joining
- ✅ Proper memory allocation and cleanup

## License

MIT License - See LICENSE file for details

## Repository

GitHub: https://github.com/Ibek7/pthreads-sync-bench-c17

---

**For detailed requirement verification, see COMPREHENSIVE_TEST_REPORT.md**
