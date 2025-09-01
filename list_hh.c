#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct node {
    int val;
    struct node* next;
    pthread_mutex_t lock;
} node_t;

node_t* make_node(int v, node_t* next) {
    node_t* n = malloc(sizeof(node_t));
    n->val = v; n->next = next; pthread_mutex_init(&n->lock, NULL);
    return n;
}

node_t* head = NULL; // sentinel

int contains(int v) {
    node_t* prev = head;
    pthread_mutex_lock(&prev->lock);
    node_t* cur = prev->next;
    while (cur) {
        pthread_mutex_lock(&cur->lock);
        if (cur->val >= v) break;
        pthread_mutex_unlock(&prev->lock);
        prev = cur;
        cur = cur->next;
    }
    int found = (cur && cur->val == v);
    if (cur) pthread_mutex_unlock(&cur->lock);
    pthread_mutex_unlock(&prev->lock);
    return found;
}

int insert(int v) {
    node_t* prev = head;
    pthread_mutex_lock(&prev->lock);
    node_t* cur = prev->next;
    while (cur) {
        pthread_mutex_lock(&cur->lock);
        if (cur->val >= v) break;
        pthread_mutex_unlock(&prev->lock);
        prev = cur;
        cur = cur->next;
    }
    if (cur && cur->val == v) {
        if (cur) pthread_mutex_unlock(&cur->lock);
        pthread_mutex_unlock(&prev->lock);
        return 0; // already present
    }
    node_t* n = make_node(v, cur);
    prev->next = n;
    if (cur) pthread_mutex_unlock(&cur->lock);
    pthread_mutex_unlock(&prev->lock);
    return 1;
}

int remove_val(int v) {
    node_t* prev = head;
    pthread_mutex_lock(&prev->lock);
    node_t* cur = prev->next;
    while (cur) {
        pthread_mutex_lock(&cur->lock);
        if (cur->val >= v) break;
        pthread_mutex_unlock(&prev->lock);
        prev = cur;
        cur = cur->next;
    }
    if (!cur || cur->val != v) {
        if (cur) pthread_mutex_unlock(&cur->lock);
        pthread_mutex_unlock(&prev->lock);
        return 0;
    }
    prev->next = cur->next;
    pthread_mutex_unlock(&cur->lock);
    pthread_mutex_destroy(&cur->lock);
    free(cur);
    pthread_mutex_unlock(&prev->lock);
    return 1;
}

// Simple benchmark: many threads insert and lookup random values
#include <time.h>
#include <unistd.h>

int NUM_THREADS = 4;
long OPS_PER_THREAD = 200000;

void* worker(void* arg) {
    int id = (int)(long)arg;
    unsigned int seed = time(NULL) ^ id;
    for (long i = 0; i < OPS_PER_THREAD; i++) {
        int v = rand_r(&seed) % 10000;
        if (i % 2) insert(v);
        else contains(v);
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc >= 2) NUM_THREADS = atoi(argv[1]);
    if (argc >= 3) OPS_PER_THREAD = atol(argv[2]);

    // initialize head sentinel
    head = make_node(-1, NULL);

    pthread_t *t = malloc(sizeof(pthread_t) * NUM_THREADS);
    struct timeval s,e;
    gettimeofday(&s, NULL);
    for (int i = 0; i < NUM_THREADS; i++) pthread_create(&t[i], NULL, worker, (void*)(long)i);
    for (int i = 0; i < NUM_THREADS; i++) pthread_join(t[i], NULL);
    gettimeofday(&e, NULL);
    long elapsed = (e.tv_sec - s.tv_sec) * 1000000L + (e.tv_usec - s.tv_usec);
    printf("threads=%d ops_per_thread=%ld time_us=%ld\n", NUM_THREADS, OPS_PER_THREAD, elapsed);
    return 0;
}
