#!/bin/bash
set -e

make

echo "Timer test:"; ./timer_test

echo "Mutex counter x1:"; ./counter_mutex 1 1000000

echo "Mutex counter x4:"; ./counter_mutex 4 1000000

echo "Approx counter x1:"; ./approx_counter 1 1000000 1000

echo "Approx counter x4:"; ./approx_counter 4 1000000 1000

echo "List hand-over-hand x4:"; ./list_hh 4 200000

echo "Hash single x4:"; ./hash_single 4 200000

echo "Hash buckets x4:"; ./hash_buckets 4 200000
