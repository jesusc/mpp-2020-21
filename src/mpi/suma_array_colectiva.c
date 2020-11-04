/**
 * Realiza la suma de los elementos de un array con
 * operaciones colectivas, en particular reduce.
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
    mysum = update(offset, chunksize + leftover, taskid);
    
    MPI_Reduce(&mysum, &sum, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
    
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

    MPI_Reduce(&mysum, NULL, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
    
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
