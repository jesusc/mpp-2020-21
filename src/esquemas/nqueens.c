
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#include "lib/vector.h"

// https://github.com/imjacobclark/vectorlib

#define N 14

typedef struct {
  int s[N];
  int fila;
} tarea;


tarea * tarea_new(int fila) {
  tarea * t = (tarea *) malloc(sizeof(tarea));
  for(int i = 0; i < N; i++)
    t->s[i] = -1;
  t->fila = fila;
  return t;
}

int valido(tarea *t, int col) {
  for(int i = 0; i < t->fila; i++) {
    if (col == t->s[i]) {
      return false;
    } else if (abs(t->s[i] - col) == (t->fila - i)) {
      return false;
    }
  }
  return true;
}

tarea * poner_reina(tarea *actual, int col)
{
  tarea *t = tarea_new(actual->fila + 1);
  // printf("Nueva tarea: %d\n", t->fila);
  memcpy(t->s, actual->s, sizeof(int) * N);
  t->s[actual->fila] = col;
  return t;
}

void print(tarea *t) {
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      if (t->s[i] == j) {
	printf("| Q ");
      } else {
	printf("|   ");
      }
    }
    printf("|\n");
  }
}

void main(int argc, char *argv[])
{
  Vector *bolsa = vector_new();

  // Iniciar la bola
  append(bolsa, tarea_new(0));
  
  while (! is_empty(bolsa)) {
    tarea * actual = (tarea *) pop(bolsa);
    
    for(int i = 0; i < N; i++) {
      if (valido(actual, i)) {
	tarea * r = poner_reina(actual, i);

	if (actual->fila < N - 1) {  
	  append(bolsa, r);
	} else {
	  //print(r);
	  //printf("Encontrado\n");
	}
      } 
    }
    
  }

}
