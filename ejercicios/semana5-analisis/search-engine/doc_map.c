#include <stdlib.h>
#include <stdio.h>
#include "doc_map.h"

DocMap *doc_map_new()
{
  DocMap *dm = (DocMap *) malloc(sizeof(DocMap));
  GHashTable *table = g_hash_table_new(g_str_hash, g_str_equal); 

  dm->word_to_int = table;
  
  return dm;
}

VocabularyCount *vocabulary_count_new(int ndocs)
{
  VocabularyCount *dc = (VocabularyCount *) malloc(sizeof(VocabularyCount));
  dc->terms = (int *) calloc(ndocs, sizeof(int));
  dc->size = 0;
  dc->capacity = ndocs;
  return dc;
}

void vocabulary_count_free(VocabularyCount *vc)
{
  free(vc->terms);
  free(vc);
}


const int *
vocabulary_count_array_get(VocabularyCount *vc)
{
  return vc->terms;
}


void vocabulary_count_resize(VocabularyCount *dc, int new_size){
  if(new_size > dc->capacity){
    dc->terms = realloc(dc->terms, sizeof(int) * new_size);
    for(int i = dc->capacity; i < new_size; i++)
      dc->terms[i] = 0;
    dc->capacity = new_size;
  }
}

int vocabulary_count_increment(VocabularyCount *vc, int term_id, int quantity){
  vocabulary_count_resize(vc, term_id + 1);
  int total = vc->terms[term_id] + quantity;
  vc->terms[term_id] = total;
  if (term_id > vc->size)
    vc->size = term_id;
  return total;
}

int vocabulary_count_size(VocabularyCount *dc){
    return dc->size;
}

gboolean vocabulary_count_is_empty(VocabularyCount *dc){
    return dc->size == 0;
}


