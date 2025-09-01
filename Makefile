CC = gcc
CFLAGS = -O2 -std=c11 -pthread -Wall -Wextra

ALL = timer_test counter_mutex approx_counter list_hh hash_single hash_buckets

all: $(ALL)

timer_test: timer_test.c
	$(CC) $(CFLAGS) timer_test.c -o timer_test

counter_mutex: counter_mutex.c
	$(CC) $(CFLAGS) counter_mutex.c -o counter_mutex

approx_counter: approx_counter.c
	$(CC) $(CFLAGS) approx_counter.c -o approx_counter

list_hh: list_hh.c
	$(CC) $(CFLAGS) list_hh.c -o list_hh

list_single: list_single.c
	$(CC) $(CFLAGS) list_single.c -o list_single

hash_single: hash_single.c
	$(CC) $(CFLAGS) hash_single.c -o hash_single

hash_buckets: hash_buckets.c
	$(CC) $(CFLAGS) hash_buckets.c -o hash_buckets

clean:
	rm -f $(ALL)
