/**
 * Muestra el uso de MPI_Pack/MPI_Unpack para empaquetar
 * y desampaquetar datos.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAX_TEXT 64

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int buff_size = sizeof(double) + sizeof(int) + sizeof(char) * MAX_TEXT;
    char * buffer = (char *) malloc(buff_size);
    
    if (rank == 0) {
      double value = 10.0;
      char *text = "hola";
      int text_len = strlen(text) + 1;      
      int position = 0;

      
      MPI_Pack(&value, 1, MPI_DOUBLE, buffer, buff_size, &position, MPI_COMM_WORLD);
      MPI_Pack(&text_len, 1, MPI_INT, buffer, buff_size, &position, MPI_COMM_WORLD);
      MPI_Pack(text, text_len, MPI_CHAR, buffer, buff_size, &position, MPI_COMM_WORLD);
      
      // position contiene ahora el tamaño real de los datos
      MPI_Bcast(buffer, position, MPI_PACKED, 0, MPI_COMM_WORLD); 
    } else {
      double value;
      char text[MAX_TEXT];
      int text_len;
      int position = 0;
      
      MPI_Bcast(buffer, buff_size, MPI_PACKED, 0, MPI_COMM_WORLD); 
      
      MPI_Unpack(buffer, buff_size, &position, &value, 1, MPI_DOUBLE, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buff_size, &position, &text_len, 1, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, buff_size, &position, text, text_len, MPI_CHAR, MPI_COMM_WORLD);

      printf("Recibido: %f y %s\n", value, text);
    }

    free(buffer);       
    
    MPI_Finalize();
}
