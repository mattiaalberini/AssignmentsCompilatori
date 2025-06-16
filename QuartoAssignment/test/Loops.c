#include <stdio.h>

int z =9;

void loops(int *a, int *b, int *c, int *d, int N, int M) {
    N=4;
    M=2;
    int i, h;
    
    for (i = 0; i < N; i++) {
        a[i] = 3;
    }

    for (i = 0; i < N; i++) {
        h = a[i] + 2;
    }
    
}