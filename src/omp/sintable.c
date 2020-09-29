#include <stdio.h>
#include <math.h>

#define SIZE 360

int main(void) {
  double sin_table[SIZE];
  
  #pragma omp parallel for
  for(int i = 0; i < SIZE; i++) 
  {
    sin_table[i] = sin(2 * M_PI * i / SIZE);
  }

  printf("sin(%d) = %.2f\n", 0, sin_table[0]);
  printf("sin(%d) = %.2f\n", 45, sin_table[45]);
  printf("sin(%d) = %.2f\n", 90, sin_table[90]);
  printf("sin(%d) = %.2f\n", 180, sin_table[180]);

  return 0;
}
