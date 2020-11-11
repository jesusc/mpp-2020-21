#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void imprimir(int rank, double *datos, int N)
{
  printf("Rank %d: ", rank);
  for(int i = 0; i < N; i++) {
    printf("%2.f ", datos[i]);
  }
  printf("\n");
}


void buscar_maximo(double *x, int N, int rank, int np)
{
  
}

void main(int argc, char *argv[])
{
  int size;
  int rank;
  int N = 10;
  double datos[N];
  
  MPI_Init(&argc,&argv);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for(int i = 0; i < N; i++) datos[i] = i + 1;
  } 
  
  buscar_maximo(datos, N, rank, size);
  
  MPI_Finalize();  
}
