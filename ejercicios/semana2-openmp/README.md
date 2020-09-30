
# Multiplicación de matrices

En este ejercicio se propone la paralelización de la múltiplicación
de dos matrices A y B, de tamaño NxN. El resultado es otra matriz C
de tamaño NxN.

La función `sec` es una versión secuencial de la multiplicación.

Tras compilar el programa puedes ejecutarlo con:
```
./main < tests/example.input  > example.output
./main < tests/test.input  > test.output
```

Para comprobar que es correcto:
```
./diff tests/example.output example.output
./diff tests/test.output test.output
```

## Cuestión 1.
Realiza la paralelización de la múltiplicación. Compara el tiempo
de ejecución y también hazlo variando el número de hilos.

Incluye en el informe los tiempos de ejecución de la versión secuencial
y de las diferentes pruebas indicando el número de hilos.

## Cuestión 2.
Consultar la cláusula “collapse” y utilizarla en este código. ¿Se obtiene mejor tiempo de ejecución que en los casos anteriores?

## Cuestión 3 (opcional)
Realizar la multiplicación utilizando la cláusula sections con cuatro
secciones. Comparar los resultados con respecto a las otras versiones paralelas.

## Entregable

* Código fuente comprimido
* Informe contestando a las preguntas en pdf

