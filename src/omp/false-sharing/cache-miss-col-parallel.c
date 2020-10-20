#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define NUM  10000

float   array[NUM][NUM];

int main( int argc, char *argv[ ] ){
  float sum = 0.;

  if (argc < 2) {
    printf("Se espera el número de hilos\n");
    return -1;
  }
  
  omp_set_num_threads(atoi(argv[1]));
  double start = omp_get_wtime();

#pragma omp parallel for shared(array) reduction(+:sum)
  for( int i = 0; i < NUM; i++ ){
    for( int j = 0; j < NUM; j++ ){
      sum += array[ j ][ i ]; // acceso por columnas (j, i)
    }
  }
  double finish = omp_get_wtime();
  double elapsed = finish - start;
  
  printf("Tiempo = %2.8f seconds\n", elapsed);
} 
