#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define TABLE_SIZE 32768

typedef struct hnode { int key; struct hnode* next; } hnode_t;

hnode_t* table[TABLE_SIZE];
pthread_mutex_t bucket_locks[TABLE_SIZE];

unsigned int hash(int k) { return (unsigned int)k % TABLE_SIZE; }

void table_init() { 
    for (int i=0;i<TABLE_SIZE;i++) { table[i]=NULL; pthread_mutex_init(&bucket_locks[i], NULL); }
}

int table_insert(int k) {
    unsigned int h = hash(k);
    pthread_mutex_lock(&bucket_locks[h]);
    hnode_t* cur = table[h];
    while (cur) { if (cur->key==k) { pthread_mutex_unlock(&bucket_locks[h]); return 0; } cur=cur->next; }
    hnode_t* n = malloc(sizeof(hnode_t)); n->key=k; n->next=table[h]; table[h]=n;
    pthread_mutex_unlock(&bucket_locks[h]);
    return 1;
}

int table_contains(int k) {
    unsigned int h = hash(k);
    pthread_mutex_lock(&bucket_locks[h]);
    hnode_t* cur = table[h];
    while (cur) { if (cur->key==k) { pthread_mutex_unlock(&bucket_locks[h]); return 1; } cur=cur->next; }
    pthread_mutex_unlock(&bucket_locks[h]);
    return 0;
}

// benchmark
int NUM_THREADS = 4; long OPS_PER_THREAD = 200000;

void* worker(void* arg) {
    int id = (int)(long)arg;
    unsigned int seed = time(NULL) ^ id;
    for (long i = 0; i < OPS_PER_THREAD; i++) {
        int v = rand_r(&seed) & 0x7fffffff;
        if (i % 2) table_insert(v);
        else table_contains(v);
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc >= 2) NUM_THREADS = atoi(argv[1]);
    if (argc >= 3) OPS_PER_THREAD = atol(argv[2]);
    table_init();
    pthread_t *t = malloc(sizeof(pthread_t)*NUM_THREADS);
    struct timeval s,e; gettimeofday(&s,NULL);
    for (int i=0;i<NUM_THREADS;i++) pthread_create(&t[i], NULL, worker, (void*)(long)i);
    for (int i=0;i<NUM_THREADS;i++) pthread_join(t[i], NULL);
    gettimeofday(&e,NULL);
    long elapsed = (e.tv_sec - s.tv_sec) * 1000000L + (e.tv_usec - s.tv_usec);
    printf("threads=%d ops_per_thread=%ld time_us=%ld\n", NUM_THREADS, OPS_PER_THREAD, elapsed);
    return 0;
}
