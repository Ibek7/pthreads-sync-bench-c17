#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

long long counter = 0;
pthread_mutex_t lock;
long iterations_per_thread = 1000000;

void* worker(void* arg) {
    long n = iterations_per_thread;
    for (long i = 0; i < n; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char** argv) {
    int threads = 4;
    if (argc >= 2) threads = atoi(argv[1]);
    if (argc >= 3) iterations_per_thread = atol(argv[2]);

    pthread_t *t = malloc(sizeof(pthread_t) * threads);
    pthread_mutex_init(&lock, NULL);

    struct timeval s,e;
    gettimeofday(&s, NULL);

    for (int i = 0; i < threads; i++) pthread_create(&t[i], NULL, worker, NULL);
    for (int i = 0; i < threads; i++) pthread_join(t[i], NULL);

    gettimeofday(&e, NULL);
    long elapsed = (e.tv_sec - s.tv_sec) * 1000000L + (e.tv_usec - s.tv_usec);

    printf("threads=%d iters_per_thread=%ld final=%lld time_us=%ld\n", threads, iterations_per_thread, counter, elapsed);

    pthread_mutex_destroy(&lock);
    free(t);
    return 0;
}
