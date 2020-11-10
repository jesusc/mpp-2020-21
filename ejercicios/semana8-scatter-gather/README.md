# Paralelización del cáculo de medias de valores

En este ejercicio se propone calcular la media de los valores de un
array en paralelo, utilizando MPI. El contenido del array se generará
con números aleatorios.

La entrada del programa es el número total de elementos por proceso.
El proceso maestro creará el array global al principio y lo repartirá
al resto de procesos con MPI_Scatter. Cada proceso realizará el cálculo
del trozo que le llega, y se devolverá al maestro con MPI_Gatter.
El maestro deberá ahora terminar de calcular la media.

Entregable:
 * Fichero avg.c


