#include <stdio.h>
#include <stdlib.h>

#define L 1024
#define N 65536
#define M (65536 * 1000)

int main() {
    //low locality
    int* m = malloc(M*sizeof(int));
    int* n = malloc(M*sizeof(int));
    long int* l = malloc(M*sizeof(long int));

    for (int i = 0; i < M; i++) {
        m[i] = i;
        n[i] = M-i;
    }

    printf("repeat %d times\n", M);

    for (int i=0; i < M; i++) {
        l[i] = m[i] * n[i];
    }

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


    printf("%d * %d = %ld\n", m[2], n[2], l[2]);
    printf("(%d + %d) * %d = %ld\n", x[2], y[2], M/L,z[2]);
    printf("(%d + %d) * %d = %ld\n", a[2], b[2], M/N, c[2]);

    return 0;
}
