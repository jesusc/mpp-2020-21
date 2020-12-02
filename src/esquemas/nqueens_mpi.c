
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#include "lib/vector.h"

// https://github.com/imjacobclark/vectorlib

//#define N 14
#define N 14

typedef struct {
  int s[N];
  int fila;
} tarea;


tarea * tarea_copia(tarea *original) {
  tarea *t = (tarea *) malloc(sizeof(tarea));
  for(int i = 0; i < N; i++) {
    t->s[i] = original->s[i];
  }
  t->fila = original->fila;
  return t;
}

tarea * tarea_new(int fila) {
  tarea * t = (tarea *) malloc(sizeof(tarea));
  for(int i = 0; i < N; i++)
    t->s[i] = -1;
  t->fila = fila;
  return t;
}

int valido(tarea *t, int col) {
  for(int i = 0; i < t->fila; i++) {
    if (col == t->s[i]) {
      return false;
    } else if (abs(t->s[i] - col) == (t->fila - i)) {
      return false;
    }
  }
  return true;
}

tarea * poner_reina(tarea *actual, int col)
{
  tarea *t = tarea_new(actual->fila + 1);
  // printf("Nueva tarea: %d\n", t->fila);
  memcpy(t->s, actual->s, sizeof(int) * N);
  t->s[actual->fila] = col;
  return t;
}

void print_short(tarea *t) {
  for(int i = 0; i < N; i++) {
    printf("%d ", t->s[i]);
  }
  printf(" fila = %d\n", t->fila);
}

void print(tarea *t) {
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      if (t->s[i] == j) {
	printf("| Q ");
      } else {
	printf("|   ");
      }
    }
    printf("|\n");
  }
}

void main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int procs;
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  // Create datatype
  // https://www.rookiehpc.com/mpi/docs/mpi_type_create_struct.php
  tarea ejemplo;

  int row_size = N;
  MPI_Datatype tarea_type;
  MPI_Datatype type[2] = { MPI_INT, MPI_INT };
  int blocklen[2] = { row_size, 1 };
  MPI_Aint addresses[2];
  MPI_Aint disp[2];

  MPI_Get_address(&ejemplo, &addresses[0]);
  MPI_Get_address(&(ejemplo.fila),&addresses[1]);

  disp[0] = 0;
  disp[1] = addresses[1] - addresses[0];
  printf("%d - %d\n", disp[0], disp[1]);
  MPI_Type_create_struct(2, blocklen, disp, type, &tarea_type);
  MPI_Type_commit(&tarea_type);
   
  if (rank == 0) {
    printf("Soy el maestro\n");
    
    Vector *bolsa = vector_new();
    
    Vector *soluciones = vector_new();
       
    // Generar un montón de tareas para repartir entre los procesos
    for(int i = 0; i < N; i++) {
      tarea *t = tarea_new(1);
      t->s[0] = i;
      append(bolsa, t);
    }

    //int *libres = (int *) calloc(sizeof(int), procs);
    //for(int i = 0; i < procs; i++)
    //libres[i] = 1;
    
    int ocupados = 0;
      
    while (! is_empty(bolsa)) {

      // Enviar trabajos
      for(int i = 1; i < procs; i++) {
	if (!is_empty(bolsa)) {
	  tarea * actual = (tarea *) pop(bolsa);

	  printf("Enviando trabajo a %d\n", i);
	  print_short(actual);
	  
	  MPI_Send(actual, 1, tarea_type, i, 0, MPI_COMM_WORLD);

	  
	  printf("Enviado trabajo a %d\n", i);
	  print_short(actual);
	  ocupados++;

	  free(actual);
	}
      }
	
	while (ocupados > 0) {
	  tarea tareas[N];
	  MPI_Status status;
	  MPI_Recv(&tareas, N, tarea_type, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
	  ocupados--;
	  
	  int count;
	  MPI_Get_count(&status, tarea_type, &count);
	  
	  printf("Recibido desde %d - %d tareas\n", status.MPI_SOURCE, count);

	  for(int j = 0; j < count; j++) {
	    
	    printf("Recibido trabajo num %d\n", j);
	    print_short(&tareas[j]);

	    tarea *t = tarea_copia(&tareas[j]);
	    if (t->fila == -2) {
	      // Es solución
	      append(soluciones, t);
	    } else {
	      append(bolsa, t);
	    }
	  }
	  
	}
		
      }

      printf("Finished %d\n", rank);

      for(int i = 1; i < procs; i++) {
	tarea marca_fin;
	marca_fin.fila = -1;
	MPI_Send(&marca_fin, 1, tarea_type, i, 0, MPI_COMM_WORLD);
      }

      MPI_Barrier(MPI_COMM_WORLD);
      
      printf("\nSoluciones %d:\n\n", size(soluciones));
      while (!is_empty(soluciones)) {
	tarea *t = (tarea *) pop(soluciones);
	print(t);
	printf("\n");
      }
      
  } else {
    while (true) {
    
      MPI_Status status;
      tarea actual;
      MPI_Recv(&actual, N, tarea_type, 0, 0, MPI_COMM_WORLD, &status);
      printf("Recibido %d\n", rank);
      print_short(&actual);

      if (actual.fila == -1)
	break;
      
      tarea *tareas = (tarea *) calloc(sizeof(tarea), N);
      int count = 0;
      
      for(int i = 0; i < N; i++) {
	if (valido(&actual, i)) {
	  tarea * r = poner_reina(&actual, i);
	  if (actual.fila < N - 1) {
	    tareas[count] = *r;
	    count++;
	    // append(bolsa, r);
	  } else {
	    tareas[count] = *r;
	    tareas[count].fila = -2;
	    count++;
	    
	    // Es solución, informar
	    printf("Encontrado\n");
	    print_short(r);

	  }
	} 
      }
      
      MPI_Send(tareas, count, tarea_type, 0, 0, MPI_COMM_WORLD);

      free(tareas);      
    }

    
    MPI_Barrier(MPI_COMM_WORLD);
  }
  
  
  MPI_Finalize();
}


