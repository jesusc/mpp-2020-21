#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DEBUG 1
#define ARRAYSIZE 100

int main ( int argc, char *argv[] )
{
  int rank, world_size;
  int *data;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  /* El maestro es el encargado de cargar los datos */
  if(rank == 0) {
      data = (int *) calloc(ARRAYSIZE, sizeof(int));
      for (int i = 0; i < ARRAYSIZE; i++)
	data[i] = i + 1;
      
      if(DEBUG) {
         printf("Datos: ");
         for (int i = 0; i < 100; i++)
	   printf(" %d",data[i]);
	 printf("\n");
      }
   }

   /* Dividir el trabajo */
   int chunksize = (ARRAYSIZE / world_size);
   int leftover = (ARRAYSIZE % world_size);

   int *tosum = (int *) malloc(chunksize * sizeof(int));
   
   /* Enviar el trozo saltandose la parte sobrante que le toca al maestro */
   MPI_Scatter(&data[leftover], chunksize, MPI_INT,
	       tosum, chunksize, MPI_INT, 0, MPI_COMM_WORLD);

   /* Cada nodo tiene 25 números para sumar */
   if(DEBUG)
   {
      printf("Nodo %d se encarga de:", rank);
      for(int i = 0; i < chunksize; i++)
	printf(" %d", tosum[i]);
      printf("\n");
   }

   int local_sum = 0;
   
   if (rank == 0) {
     for(int i = 0; i < leftover; i++)
       local_sum += data[i];
   }
   
   for(int i = 0; i < chunksize; i++)
     local_sum += tosum[i];

   if(DEBUG)
     printf("Nodo %d calcula la suma %d\n", rank, local_sum);

   int sum; 
   MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);

   /* El maestro recibe los resultados parciales y los suma */
   if(rank == 0) 
   {
      printf("Resultado %d\n", sum);
      free(data);
   }

   free(tosum);
   
   MPI_Finalize();
   return 0;
}
