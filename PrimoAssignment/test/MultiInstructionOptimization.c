int multiInstructionOptimization(int x) {
         
    int a = x + 1;
    int b = a - 1;
  
    int c = x - 2;
    int d = c + 2;

    int e = 3 * x;
    int f = e / 3;
    
    int g = x / 4;
    int h = 4 * g; 

    return b + d + f + h;
}