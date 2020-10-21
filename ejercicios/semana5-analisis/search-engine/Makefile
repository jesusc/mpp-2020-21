CC=gcc
CFLAGS=-g -ggdb -DPROFILE=3000 -Wall
#CFLAGS=-g -ggdb -Wall
GLIB_FLAGS=`pkg-config --cflags --libs glib-2.0`

index_slow: index-slow.c build/sqlite3.o build/vector.o build/utils.o build/db.o
	$(CC) -o index_slow $^ -lpthread -ldl $(CFLAGS) 

#index: index.c build/sqlite3.o build/vector.o build/utils.o build/db.o
#	$(CC) -o index $^ -lpthread -ldl $(CFLAGS) 

index_memory: index-memory.c build/vector.o build/utils.o build/doc_map.o
	$(CC) -o index-memory $^ -lpthread -ldl $(CFLAGS) $(GLIB_FLAGS)

query: query.c build/sqlite3.o build/vector.o build/utils.o build/db.o
	$(CC) -o query $^ -lpthread -ldl $(CFLAGS) $(GLIB_FLAGS)

build/sqlite3.o: lib/sqlite/sqlite3.c
	$(CC) -c -o build/sqlite3.o lib/sqlite/sqlite3.c $(CFLAGS) 

build/vector.o: lib/vector/vector.c
	$(CC) -c -o build/vector.o lib/vector/vector.c $(CFLAGS) 

build/utils.o: utils.c
	$(CC) -c -o build/utils.o utils.c $(CFLAGS) 

build/db.o: db.c
	$(CC) -c -o build/db.o db.c $(CFLAGS) 

build/doc_map.o: doc_map.c
	$(CC) -c -o build/doc_map.o doc_map.c $(CFLAGS) $(GLIB_FLAGS)

.PHONY: clean
clean:
	rm -rf build/*.o index query index_slow *~
