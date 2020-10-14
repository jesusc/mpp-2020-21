CC=gcc
CFLAGS=-g -ggdb -DPROFILE=3000 -Wall
GLIB_FLAGS=`pkg-config --cflags --libs glib-2.0`

all: tree tree_omp

tree: tree.c
	$(CC) -o tree $^ -lm -fopenmp $(CFLAGS) $(GLIB_FLAGS)

tree_omp: tree_omp.c
	$(CC) -o tree_omp $^ -lm -fopenmp $(CFLAGS) $(GLIB_FLAGS)

.PHONY: clean
clean:
	rm -rf main *~ tree tree_omp
