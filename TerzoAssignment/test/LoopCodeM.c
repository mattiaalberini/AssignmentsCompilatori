#include <stdio.h>

void foo(int c, int z) {
  int a = 3, h, m = 0, n = 3, q, r = 0, y = 0;

LOOP:
  z = z + 1;
  y = c + 3;
  q = c + 7;
  if (z < 5) {
    a = a + 2;
    h = c + 3;
  } else {
    a = a - 1;
    h = c + 4;
    if (z >= 6) {
      goto EXIT;
    }
  }
  m = y + 7;
  n = h + 2;
  y = c + 7;
  r = q + 5;
  goto LOOP;
EXIT:
}

int main() {
  foo(0, 5);
  foo(0, 6);
  return 0;
}