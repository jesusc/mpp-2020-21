
# Profiling

El programa `index_slow` indexa documentos de texto en una base de datos
SQLite3, pero tiene dos cuellos de botella relacionados con la base de
datos. También hay partes de la implementación que podrían optimazarse.

En este ejercicio se deben utilizar profilers para identificar los
cuellos de botella.

Se debe entregar un informe breve que incluya:

 * Qué estrategia de profiling se ha usado y qué partes del código tardan más tiempo.
 * Cuales son cuellos de botella más relevantes y posibles soluciones.
 * Qué partes del código podrían paralelizarse y cómo.

La implementación de código para arreglar los cuellos de botella y la
paralelización es opcional. 