/********************************************************************
* Francisco Almeida, Domingo Giménez, José Miguel Mantas, Antonio M. Vidal:
* Introducción a la programación paralela,
* Paraninfo Cengage Learning, 2008
*
* Capítulo 3, Código 3.12
* La directiva parallel
*********************************************************************/

#include <stdio.h>
#include <omp.h>

int main ()  {
    int nthreads, tid;
    printf("Trabajemos con 4 threads\n");
    omp_set_num_threads(4);
    // Obtiene el número de threads en ejecución
    nthreads = omp_get_num_threads();
    printf("Numero de threads en ejecucion = %d\n", nthreads);

    #pragma omp parallel private(tid)       // Expande un grupo de threads
               // Cada thread contiene sus propias copias de variables
    {
        tid = omp_get_thread_num(); // Obtiene el identificador del thread 
        printf("Hola desde el thread = %d\n", tid);
        if (tid == 0) { // Esta sección la ejecuta sólo el thread maestro
            nthreads = omp_get_num_threads();
            printf("Numero de threads = %d\n", nthreads);
        }
    }  // Todos los threads se unen al thread maestro y finalizan

    printf("Trabajemos ahora con 3 threads \n");

    omp_set_num_threads(3);
    nthreads = omp_get_num_threads();
    printf("Numero de threads en ejecucion = %d\n", nthreads);
    #pragma omp parallel
    {
        tid = omp_get_thread_num();
        printf("Hola desde el thread = %d\n", tid);
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Numero de threads = %d\n", nthreads);
        }
    }
}
