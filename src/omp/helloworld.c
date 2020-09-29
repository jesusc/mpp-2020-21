#include <stdio.h>

int main(void) {
  printf("Hello\n");
  #pragma omp parallel
  {
    // El código del bloque se ejecuta en paralelo
    printf("world!\n");
  }

  return 0;
}
