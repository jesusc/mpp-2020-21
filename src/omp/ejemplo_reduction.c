#include <omp.h>


#include <stdio.h>
int main()  
{                                 
        int x=1000;
	int iam =0, np = 1, i=0,j=0;     
	
	printf("\n Antes de pragma parallel x=%d \n\n",x);
	
	#pragma omp parallel private(iam, np,i) reduction(+:x)
    	{
		#if defined (_OPENMP) 
      		  np = omp_get_num_threads(); 
      		  iam = omp_get_thread_num();
                #endif
		//printf("Hello from thread %d out of %d \n",iam,np);

         
                  printf("Soy el thread %d, antes de actualizar, con x=%d \n",iam,x);  // mensajito para que le de tiempo al master a poner x=999             
                  x=iam*10;
                  printf("\t\tSoy el thread %d, despues de actualizar, con x=%d \n",iam,x); 
                  //todos se quedan con el valor que ponga el thread que haga la iteracion i=10
                
                
        }//parallel
        
        printf("\n Despues de pragma parallel x=%d \n\n",x);
}//main

