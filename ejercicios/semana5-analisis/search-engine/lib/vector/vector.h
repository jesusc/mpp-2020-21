#include <stdbool.h>
#define VECTOR_INITIAL_CAPACITY 16

typedef struct {
    int size;
    int capacity;
    void **data;
} Vector;

Vector * vector_new();
Vector * vector_new_with_capacity(int capacity);

void vector_free(Vector *v);

void vector_init(Vector *vector, int capacity);
void vector_append(Vector *vector, void * value);
void vector_prepend(Vector *vector, void * value);
void vector_delete(Vector *vector, int index);
void vector_delete_value(Vector *vector, void * value);
void vector_set(Vector *vector, int index, void * value);
void vector_resize(Vector *vector);
void vector_free_memory(Vector *vector);

void * vector_get(Vector *vector, int value);
void * vector_pop(Vector *vector);
int vector_find_value(Vector *vector, void * value);
int vector_size(Vector *vector);
int vector_capacity(Vector *vector);

bool vector_is_empty(Vector *vector);
