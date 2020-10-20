#include <stdio.h>
#include <omp.h>

#define NUM  10000

float   array[NUM][NUM];

int main( int argc, char *argv[ ] ){
  float sum = 0.;

  double start = omp_get_wtime();

  for( int i = 0; i < NUM; i++ ){
    for( int j = 0; j < NUM; j++ ){
      sum += array[ j ][ i ]; // acceso por columnas (j, i)
    }
  }
  double finish = omp_get_wtime();
  double elapsed = finish - start;
  
  printf("Tiempo = %2.8f seconds\n", elapsed);
} 
