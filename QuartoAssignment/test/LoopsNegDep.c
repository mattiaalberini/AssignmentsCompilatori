
void loops(int *a, int *b, int *c, int *d, int N, int M) {
    int i = 0;

    for (i = 0; i < N; i++) {
        a[i] = 3;
    }

    for (i = 0; i < N; i++) {
        b[i] = a[i + 2] + 2;
    }
}