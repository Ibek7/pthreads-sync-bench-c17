#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdatomic.h>

atomic_llong global = 0;
long iterations_per_thread = 1000000;
long threshold = 1000;
int num_threads = 4;

typedef struct { long local; int id; } thread_arg_t;

void* worker(void* arg) {
    thread_arg_t *a = arg;
    long local = 0;
    for (long i = 0; i < iterations_per_thread; i++) {
        local++;
        if (local >= threshold) {
            atomic_fetch_add(&global, local);
            local = 0;
        }
    }
    if (local > 0) atomic_fetch_add(&global, local);
    return NULL;
}

int main(int argc, char** argv) {
    if (argc >= 2) num_threads = atoi(argv[1]);
    if (argc >= 3) iterations_per_thread = atol(argv[2]);
    if (argc >= 4) threshold = atol(argv[3]);

    pthread_t *t = malloc(sizeof(pthread_t) * num_threads);
    thread_arg_t *args = malloc(sizeof(thread_arg_t) * num_threads);

    struct timeval s,e;
    gettimeofday(&s, NULL);

    for (int i = 0; i < num_threads; i++) {
        args[i].local = 0; args[i].id = i;
        pthread_create(&t[i], NULL, worker, &args[i]);
    }
    for (int i = 0; i < num_threads; i++) pthread_join(t[i], NULL);

    gettimeofday(&e, NULL);
    long elapsed = (e.tv_sec - s.tv_sec) * 1000000L + (e.tv_usec - s.tv_usec);

    printf("threads=%d iters_per_thread=%ld threshold=%ld global=%lld time_us=%ld\n",
           num_threads, iterations_per_thread, threshold, (long long)atomic_load(&global), elapsed);

    free(t); free(args);
    return 0;
}
