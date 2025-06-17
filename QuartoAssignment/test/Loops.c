void loops(int *a, int *b, int *c, int *d, int N, int M) {
    N=4;
    M=2;
    int i;
    
    for (i = 0; i < N; i++) {
        a[i] = 3;
    }

    for (i = 0; i < N; i++) {
        b[i] = a[i] + 2;
    }
}