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


void intercambiar(double *x, int N, int p1, int p2)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == p1) {
    MPI_Send(x, N, MPI_DOUBLE, p2, 100, MPI_COMM_WORLD);
    MPI_Recv(x, N, MPI_DOUBLE, p2, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else {
    double *buffer = (double *) malloc(N * sizeof(double));
    MPI_Recv(buffer, N, MPI_DOUBLE, p1, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(x, N, MPI_DOUBLE, p1, 100, MPI_COMM_WORLD);
    memcpy(x, buffer, N);
    free(buffer);  
  }
}

void main(int argc, char *argv[])
{
  int size;
  int rank;
  int N = 10;
  double datos[N];
  
  MPI_Init(&argc,&argv);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2) {
    printf("Solo dos procesos permitidos\n");
    return;
  }
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for(int i = 0; i < N; i++) datos[i] = i + 1;
  } else {
    for(int i = 0; i < N; i++) datos[i] = N - i;
  }

  imprimir(rank, datos, N);
  
  intercambiar(datos, N, 0, 1);

  imprimir(rank, datos, N);
  
  MPI_Finalize();  
}
