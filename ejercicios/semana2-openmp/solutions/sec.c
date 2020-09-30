/*
  CPP_CONTEST=PP-FP2014
  CPP_PROBLEM=B
  CPP_LANG=C+OPENMP
  CPP_PROCESSES_PER_NODE=saturno 1
*/

#include <stdlib.h>
#include <omp.h>

//El número de hilos está a 1 para la ejecución secuencial
//se puede variar para utilizar más hilos
#define NUM_THREADS 1
    
void  sec(int n,double *a,double *b,double *c)
{
  int i, j, k;
  double s;

//Código 6.6

   omp_set_num_threads(NUM_THREADS);
   #pragma omp parallel for private(i, j, k, s)
// cada thread trabaja con un bloque de filas de la matriz a
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            s = 0.;
            for (k = 0; k < n; k++)
                s += a[i * n + k] * b[k * n + j];
            c[i * n + j] = s;
        }
}

