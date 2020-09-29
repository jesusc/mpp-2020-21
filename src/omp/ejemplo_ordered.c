#include <omp.h>


#include <stdio.h>
int main()  
{                                 
	int iam =0, np = 1, i=0,j=0;     
	#pragma omp parallel private(iam, np,i)
    	{
		#if defined (_OPENMP) 
      		  np = omp_get_num_threads(); 
      		  iam = omp_get_thread_num();
                #endif
		//printf("Hello from thread %d out of %d \n",iam,np);

		#pragma omp for ordered
                for(i=0;i<5;i++)
                {
                  printf("\t\tSoy el thread %d, antes del ordered en la iteracion %d\n",iam,i);
                  
                  #pragma omp ordered
                  {
                    printf("Soy el thread %d, actuando en la iteracion %d\n",iam,i);
                    sleep(1);
                  }
                  
                }
                
        }//parallel
}//main

