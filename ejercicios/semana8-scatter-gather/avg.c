#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

float *generar_aleatorios(int num_elementos) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elementos);
  for (int i = 0; i < num_elementos; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

// Calcula la media de un número de elementos
float calcular_media(float *array, int num_elementos) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elementos; i++) {
    sum += array[i];
  }
  return sum / num_elementos;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Uso: avg num_elementos_proc\n");
    exit(1);
  }

  int num_elementos_proc = atoi(argv[1]);
  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = generar_aleatorios(num_elementos_proc * world_size);
  }

  /* TODO: Repartir los datos entre todos los procesos usando MPI_Scatter */
  
  /* TODO: Cada proceso calcula la media con calcular_media */

  /* TODO: Enviar las medias parciales al proceso maestro, que debe estar preparado para recibirlas */

  /* TODO: El proceso maestro ha recibido las medias y ahora calcula la media de todas ellas e imprime el resultado */
  
  
  MPI_Finalize();
}
