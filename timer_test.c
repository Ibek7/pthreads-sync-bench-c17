#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>

int main(void) {
    struct timeval t1, t2;
    long min_diff = 1000000000L;
    long zeros = 0;
    int trials = 1000000;
    for (int i = 0; i < trials; i++) {
        gettimeofday(&t1, NULL);
        gettimeofday(&t2, NULL);
        long diff = (t2.tv_sec - t1.tv_sec) * 1000000L + (t2.tv_usec - t1.tv_usec);
        if (diff == 0) zeros++;
        else if (diff > 0 && diff < min_diff) min_diff = diff;
    }
    printf("trials=%d zeros=%ld smallest_nonzero=%ld microseconds\n", trials, zeros, min_diff == 1000000000L ? -1 : min_diff);

    // Quick sanity: measure a 1ms sleep using gettimeofday
    gettimeofday(&t1, NULL);
    struct timeval sleep_tv = {0, 1000}; // 1ms
    select(0, NULL, NULL, NULL, &sleep_tv);
    gettimeofday(&t2, NULL);
    long measured = (t2.tv_sec - t1.tv_sec) * 1000000L + (t2.tv_usec - t1.tv_usec);
    printf("measured sleep ~%ld microseconds\n", measured);
    return 0;
}
