/**
 * Realiza la suma de los elementos de un array sin utilizar
 * operaciones colectivas
 */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  ARRAYSIZE	20000000
#define  MASTER		0

double  data[ARRAYSIZE];

int main (int argc, char *argv[])
{
  int   numtasks, taskid, rc, dest, offset, i, j, tag1,
    tag2, source, chunksize, leftover; 
  double mysum, sum;
  double update(int myoffset, int chunk, int myid);
  MPI_Status status;
  
  /***** Initializations *****/
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

  printf ("MPI task %d has started...  ", taskid);
  chunksize = (ARRAYSIZE / numtasks);
  leftover = (ARRAYSIZE % numtasks);
  tag2 = 1;
  tag1 = 2;
  
  if (taskid == MASTER){

    /* Inicialización de los datos. En un ambiente más
       real se tomarían de un fichero u otra fuente */
    sum = 0;
    for(i=0; i<ARRAYSIZE; i++) {
      data[i] =  i * 1.0;
      sum = sum + data[i];
    }
    
    printf("Initialized array sum = %e\n",sum);
    printf("numtasks= %d  chunksize= %d  leftover= %d\n",numtasks,chunksize,leftover);

    /* Master: envía a cada proceso su parte del trabajo */
    offset = chunksize + leftover;
    for (dest = 1; dest < numtasks; dest++) {
      MPI_Send(&data[offset], chunksize, MPI_DOUBLE, dest, tag2, MPI_COMM_WORLD);
      printf("Enviados %d elementos al proceso %d, offset= %d\n",
	     chunksize,dest,offset);
      offset = offset + chunksize;
    }
    
    /* Master does its part of the work */
    offset = 0;
    sum = update(offset, chunksize + leftover, taskid);
    
    /* Esperar a recibir el resultado del resto de tareas, una a una
       y en orden. */
    for (i=1; i<numtasks; i++) {
      source = i;
      MPI_Recv(&mysum, 1, MPI_DOUBLE, source, tag1, MPI_COMM_WORLD, &status);

      /* Agregar el resultado parcial */
      sum += mysum;
    }
    
    printf("*** Resultado final= %e ***\n",sum);
    
  } else {
    /* Procesos esclavos (no master) */

    source = MASTER;
    MPI_Recv(&data[offset], chunksize, MPI_DOUBLE, source, tag2, 
	     MPI_COMM_WORLD, &status);
    
    /* Do my part of the work */
    mysum = update(offset, chunksize, taskid);
    
    /* Send my results back to the master task */
    dest = MASTER;
    MPI_Send(&mysum, 1, MPI_DOUBLE, dest, tag1, MPI_COMM_WORLD);
    
  } /* end of non-master */
  

  MPI_Finalize();

}   /* end of main */


double update(int myoffset, int chunk, int myid) {
  int i; 
  double mysum;
  /* Perform addition to each of my array elements and keep my sum */
  mysum = 0;
  for(i=myoffset; i < myoffset + chunk; i++) {
    mysum = mysum + data[i];
  }

  printf("Task %d mysum = %e\n",myid,mysum);
  return(mysum);
}
