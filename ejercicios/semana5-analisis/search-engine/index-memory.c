/**
 * Search engine implementation completely in memory.
 */ 
#include <stdio.h>
#include <stdlib.h>

#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>

#include <assert.h>

#include <glib.h>

#include "utils.h"
#include "lib/vector/vector.h"
#include "doc_map.h"

#define BUFFER_SIZE 2048
#define DEBUG_ 0

#ifdef PROFILE
  static long long time_start = -1;
  static long total_words = 0;
#endif


typedef struct {
  char *name;
  VocabularyCount *vocabulary_count;
} Document;

typedef struct {
  GHashTable *str_to_int;
  char **terms_by_id;
} Vocabulary;


Vocabulary * vocabulary_new()
{
  Vocabulary *v = malloc(sizeof(Vocabulary));
  v->str_to_int = g_hash_table_new_full(g_str_hash, g_str_equal, &free, NULL);
  v->terms_by_id = NULL;
  return v;
}

void
vocabulary_free(Vocabulary *vocabulary)
{
  g_hash_table_destroy(vocabulary->str_to_int);
  if (vocabulary->terms_by_id != NULL)
    free(vocabulary->terms_by_id);
  free(vocabulary);
}

int
vocabulary_size(Vocabulary *vocabulary)
{
  return g_hash_table_size(vocabulary->str_to_int);
}

const char *
vocabulary_get_term(Vocabulary *vocabulary, int idx)
{
  assert(vocabulary->terms_by_id != NULL);
  return vocabulary->terms_by_id[idx];
}

void
vocabulary_commit(Vocabulary *vocabulary)
{
  GHashTableIter iter;
  gpointer key;
  gpointer value;

  if (vocabulary->terms_by_id != NULL)
    free(vocabulary->terms_by_id);
  
  vocabulary->terms_by_id = (char **) malloc(g_hash_table_size(vocabulary->str_to_int) * sizeof(char *));
  g_hash_table_iter_init(&iter, vocabulary->str_to_int);
  while(g_hash_table_iter_next(&iter, &key, &value)) {
    char *term = (char *) key;
    int id = GPOINTER_TO_INT(value);
    if (DEBUG_)
      printf("%d - %s\n", id, term);
    vocabulary->terms_by_id[id] = term;
  }
}

int
vocabulary_get_or_insert(Vocabulary *vocabulary, char *term)
{
  gpointer p_id;
  
  if (g_hash_table_lookup_extended(vocabulary->str_to_int, term, NULL, &p_id)) {
    // We have key and we increment the count
    return GPOINTER_TO_INT(p_id);
  } else {
    int size = vocabulary_size(vocabulary);
    p_id = GINT_TO_POINTER(size); //terms ids start at 0
    g_hash_table_insert(vocabulary->str_to_int, g_strdup(term), p_id);
    return size;
  }
}

Document * document_new(char *filename)
{
  Document *d = malloc(sizeof(Document));
  d->name = g_strdup(filename);
  d->vocabulary_count = vocabulary_count_new(32);
  return d;
}

void
document_free(Document *d)
{
  vocabulary_count_free(d->vocabulary_count);
  free(d->name);
  free(d);
}

void
document_print(Document *doc, Vocabulary *vocabulary)
{
  printf("Document '%s':\n", doc->name);
  const int size = vocabulary_count_size(doc->vocabulary_count);
  const int *counts = vocabulary_count_array_get(doc->vocabulary_count);

  for(int i = 0; i < size; i++) {
    const char *term = vocabulary_get_term(vocabulary, i);  
    printf("  %d %s = %d\n", i, term, counts[i]);
  }
}

void
insert_term(char *term, Document *doc, Vocabulary *vocabulary)
{
  int term_id = vocabulary_get_or_insert(vocabulary, term);
  vocabulary_count_increment(doc->vocabulary_count, term_id, 1);
}

/**
 * This function is not thread safe! 
 */
Document *
process_file(char *filename, Vocabulary *vocabulary)
{
  FILE *input_file = fopen(filename, "r");
  char buffer[BUFFER_SIZE];
  char *last_word;
  char *delimiter_characters = SEARCH_DELIMITER_CHARACTERS;

  Document *doc = document_new(filename);
  
#ifdef PROFILE
  long time_diff;
  if (time_start == -1)
    time_start = mseconds();
#endif
  
  if( input_file == NULL ){
    fprintf( stderr, "Unable to open file %s\n", filename );
  } else {
    int position = 0;

    // Read each line into the buffer
    while( fgets(buffer, BUFFER_SIZE, input_file) != NULL ){
      last_word = strtok(buffer, delimiter_characters );
      while( last_word != NULL ){
	insert_term(last_word, doc, vocabulary);
	last_word = strtok(NULL, delimiter_characters);
	position++;

  // Profiling
#ifdef PROFILE	
	total_words++;
	if (total_words % PROFILE == 0) {
	  time_diff = mseconds() - time_start;
	  if (time_diff > 0)
	    printf("Rate: %ld words/ms\n", (total_words / time_diff));
	}
#endif	
      }      
    }

    fclose(input_file);
  }
  return doc;
}

int main(int argc, char **argv)
{
  char * index_resources;

  DIR *d;
  struct dirent *dir;

  if (argc != 2) {
    printf("Usage: ./index folder\n");
    return -1;
  }

  index_resources = argv[1];
  
  d = opendir(index_resources);
  if (! d) {
    fprintf(stderr, "Invalid folder: %s\n", index_resources);
  }

  Vocabulary *vocabulary = vocabulary_new();
  
  int count = 0;
  while ((dir = readdir(d)) != NULL) {
    if (dir->d_type == DT_REG) {
      char *filename = (char *) malloc(strlen(index_resources) + strlen(dir->d_name) + 2);

      strcpy(filename, index_resources);
      strcat(filename, "/");  /* DIR_SEPARATOR = / */
      strcat(filename, dir->d_name); /* Adds '\0' automatically */
	
      printf("Processing %s\n", filename);
      Document *doc = process_file(filename, vocabulary);
      count++;

      vocabulary_commit(vocabulary);
      if (DEBUG_)
	document_print(doc, vocabulary);

      document_free(doc);
      
      free(filename);

      /*      
#ifdef PROFILE     
      if (count == 3)
	break;
#endif
      */
    } 
  }

  vocabulary_free(vocabulary);
  
  closedir(d);
  return 0;  
}
