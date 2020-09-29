#include <stdio.h>

void main(void) {
  int v = 0;
  const int n = 100;
  
  #pragma omp parallel for  
  for(int i = 0;i <= n;i++) {
    if (i % 2 == 0) {     
      v += i;
    }
  }

  printf("v = %d\n", v);
}
