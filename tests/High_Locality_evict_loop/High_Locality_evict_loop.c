#include <stdio.h>
#include <stdlib.h>

#define N 65536
#define M (65536 * 1000)

int main() {
    printf("High Locality get evicted!\n");
    //high locality, get evicted
    //increase N to get evicted
    int* a = malloc(N*sizeof(int));
    int* b = malloc(N*sizeof(int));
    long int* c = malloc(N*sizeof(long int*));
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = N-i;
    }

    printf("repeat %d * %d times\n", M/N, N);

    // data will be in the cache, look at the CPI,
    // IPC is high then compute bounded.
    for (int j =0; j < (M/N); j++)
        for (int i = 0; i < N; i++) {
            c[i] += a[i]+b[i];
        }

    printf("(%d + %d) * %d = %ld\n", a[2], b[2], M/N, c[2]);

    return 0;
}
