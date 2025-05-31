int loops(int *a, int *b, int *c, int *d, int N) {
    int i;

    for(i = 0; i < N; i++) {
        a[i] = 1 / b[i] * c[i];
    }

    for(i = 0; i < N; i++) {
        d[i] = a[i + 2] + c[i];
    }
    
    return N;
}