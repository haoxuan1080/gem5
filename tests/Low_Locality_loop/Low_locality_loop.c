#include <stdio.h>
#include <stdlib.h>

#define M (65536 * 1000)

int main() {
    printf("Low Locality loop!\n");
    //low locality
    int* m = malloc(M*sizeof(int));
    int* n = malloc(M*sizeof(int));
    long int* l = malloc(M*sizeof(long int));

    for (int i = 0; i < M; i++) {
        m[i] = i;
        n[i] = M-i;
    }

    //printf("repeat %d times\n", M);

    for (int i=0; i < M; i++) {
        l[i] = m[i] * n[i];
    }

    printf("%d * %d = %ld\n", m[2], n[2], l[2]);

    return 0;
}
