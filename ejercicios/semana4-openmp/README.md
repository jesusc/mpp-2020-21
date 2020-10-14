
# Paralelización de construcción y recorrido de árboles

Paraleliza las funciones `generate_nodes` y `sum` del programa `tree_omp.c`.
Utiliza tareas de OpenMP para ello:

 * Directiva #pragma omp task
 * Utiliza el modificador if para limitar la cantidad de tareas que se generan.
 * Directiva #pragma omp taskwait cuando sea necesario

El programa utiliza la librería GLib para la estructura de datos GNode
(árbol en memoria). La documentación está en https://developer.gnome.org/glib/2.26/glib-N-ary-Trees.html. Para instalar GLib hay que hacer:

```
sudo apt-get install libglib2.0-dev
```

Haz diversas pruebas variando la forma de utilizar tareas. Entrega el
código desarrollado y una explicación breve de lo realizado, en
particular discutiendo acerca del efecto de usar tareas para obtener
mejores (o peores) tiempos de ejecución. No es necesario en este
ejercicio realizar un informe detallado de las mejoras obtenidas.

Nota: El programa `tree.c` es una versión que utiliza números
aleatorios, pero no hay que paralelizarla.

