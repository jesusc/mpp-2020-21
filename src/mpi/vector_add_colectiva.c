
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double *generar(int n) {
  double *numeros = (double *) malloc(n*sizeof(double));  
  for(int i = 0; i < n; i++) {
    // numeros[i] = rand() * 1000;
    numeros[i] = i;
  }
  return numeros;
}
 
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // int n = 1024;
    int n = 16;
    int trozos = n / world_size;
    
    if (n % world_size != 0) {
      printf("Número de procesos inválido.\n");
      exit(-1);
    }

    // Operandos
    double *local_a = malloc(trozos * sizeof(double));
    double *local_b = malloc(trozos * sizeof(double));
    // Resultado
    double *local_c = malloc(trozos * sizeof(double));
    
    // Repartir los datos
    if (rank == 0) {
      double *a = generar(n);
      double *b = generar(n);
      
      MPI_Scatter(a, trozos, MPI_DOUBLE, local_a, trozos, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Scatter(b, trozos, MPI_DOUBLE, local_b, trozos, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      free(a);
      free(b);
    } else {
      MPI_Scatter(NULL, trozos, MPI_DOUBLE, local_a, trozos, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Scatter(NULL, trozos, MPI_DOUBLE, local_b, trozos, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Todos suman
    for(int i = 0; i < trozos; i++) {
      local_c[i] = local_a[i] + local_b[i];
    }

    // Devolver los datos al maestro
    if (rank == 0) {
      double *r = (double *) malloc(n * sizeof(double));
      
      MPI_Gather(local_c, trozos, MPI_DOUBLE, r, trozos, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      for(int i = 0; i < n; i++) {
	printf("%2.f\n", r[i]);
      }
      
      free(r);
    } else {
      MPI_Gather(local_c, trozos, MPI_DOUBLE, NULL, trozos, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    
    free(local_a);
    free(local_b);
    free(local_c);
    
    MPI_Finalize();
}
