void loops(int *a, int *b, int N) {
    N=4;
    int i=0;
    
    if(N > 2) {
        do {
            a[i] = 3;
            i += 1;
        } while (i < N);
    }
    
    
    i=0;
    if(N > 2) {
        do {
            b[i] = 2;
            i += 1;
        } while (i < N);
    }
}