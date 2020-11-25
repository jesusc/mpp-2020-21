
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define N 10
#define NUM_ITERACIONES 20000
#define UMBRAL 0.001

void iterar(double *a, double *b, int n) {
  int i, j;

  #pragma omp parallel for private (i, j)
  for(i = 1; i < n - 1; i++) {
    for(j = 1; j < n - 1; j++) {
      b[i * n + j] =
	(a[(i - 1) * n + j] +
	 a[(i + 1) * n + j] +
	 a[i * n + j - 1] +
	 a[i * n + j + 1]) / 4.0;
    }
  }
}

void jacobi(double *a, double *b, int num_iteraciones, int n) {

  int it, i, j;
  double s;
  
  for(it = 0; it < num_iteraciones; it++) { 
    s = 0.0;

    iterar(a, b, n);
    iterar(b, a, n);

    #pragma omp parallel for private (i, j) reduction(+:s)
    for(i = 1; i < n - 1; i ++) {
      for(j = 1; j < n - 1; j++) {
	s += fabs(a[i * n + j] - b[i * n + j]);
      }
    }

  printf("distancia = %lf\n", s);

    if (s < UMBRAL) {
      break;
    }
  }

}

void print_matriz(double *a, int n) {
  for(int i = 0; i < n; i ++) {
    for(int j = 0; j < n; j++) {
      printf(" %.2f", a[i * n + j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  double *V = malloc(N * N * sizeof(double));
  double *AUX = malloc(N * N * sizeof(double));
  
  int i, j;

  omp_set_num_threads(4);
  
  for (i = 1; i < N - 1; i++) {
    for (j = 1; j < N - 1; j++) {
      V[i * N + j] = 0.0;
    }
  }
  
  for (j = 0; j < N; j++) V[0 * N + j] =  30;        // arriba
  for (j = 0; j < N; j++) V[(N - 1) * N + j] =  70;  // abajo
  for (i = 0; i < N; i++) V[i * N + 0] =   0;   // izquierda
  for (i = 0; i < N; i++) V[i * N + (N - 1)] = 100;   // derecha

  memcpy(AUX, V, sizeof(double) * N * N);
  
  print_matriz(V, N);
  
  printf("----------\n");
  jacobi(V, AUX, NUM_ITERACIONES, N);
  // print_matriz(V, N);
  // printf("\n");
  //print_matriz(AUX, N);
  
	printf("\n");
  print_matriz(V, N);

  free(V);
  free(AUX);
  
  return -1;
}
