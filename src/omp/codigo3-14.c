/********************************************************************
* Francisco Almeida, Domingo Giménez, José Miguel Mantas, Antonio M. Vidal:
* Introducción a la programación paralela,
* Paraninfo Cengage Learning, 2008
*
* Capítulo 3, Código 3.14
* La directiva section
*********************************************************************/

#include <stdio.h>
#include <omp.h>

int main () {
    int nthreads, tid;
    // La variable tid es privada a cada thread 
    #pragma omp parallel private(tid)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d, de %d, calcula la seccion 1\n", tid, nthreads); 
            }
            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d, de %d, calcula la seccion 2\n", tid, nthreads); 
            }
            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d, de %d, calcula la seccion 3\n", tid, nthreads); 
            }
            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d, de %d, calcula la seccion 4\n", tid, nthreads); 
            }
        }  // fin de las sections
    }  // fin de la parallel section
}
