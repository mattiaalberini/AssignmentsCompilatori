int loop(int a, int b, int c, int d, int e, int N) {
    int i = 0, x, y; 

    do {
        a = d + 1;
        b = d + 2;

        if(d < N) {
            c = d + 3;
            if(e < N) break;
        } 
        x = d + 4;
        y = d + 5;
        
        i++;
    } while(i < N);

    c = b + 6;
    x = x + 7;

    return c;
}