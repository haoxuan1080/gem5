#include <stdio.h>
#include <stdlib.h>

#define L 1024
#define N 65536
#define M (65536 * 1000)

int main() {
    printf("High Locality loop!\n");
    //hgigh locality, not get evicted
    int* x = malloc(L*sizeof(int));
    int* y = malloc(L*sizeof(int));
    long int* z = malloc(L*sizeof(long int));
    for (int i = 0; i < L; i++) {
        x[i] = i;
        y[i] = N-i;
    }

    printf("repeat %d * %d times\n", M/L, L);

    // data will be in the cache, look at the CPI,
    // IPC is high then compute bounded.
    for (int j =0; j < (M/L); j++)
        for (int i = 0; i < L; i++) {
        //what if a call here?
        //in the report discussion
            z[i] += x[i]+y[i];
    }

    printf("(%d + %d) * %d = %ld\n", x[2], y[2], M/L,z[2]);

    return 0;
}
