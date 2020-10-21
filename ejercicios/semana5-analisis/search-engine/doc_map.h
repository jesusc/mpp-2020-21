#ifndef _DOC_MAP
#define _DOC_MAP

#include <glib.h>

/**
 * Keeps a correspondence between words and contiguous ids.
 */
typedef struct {
  GHashTable *word_to_int;
} DocMap;

typedef struct {
  int *terms;
  int size;
  int capacity;
} VocabularyCount;

DocMap *new_doc_map();

VocabularyCount *vocabulary_count_new(int capacity);
void vocabulary_count_free(VocabularyCount *vc);

int vocabulary_count_size(VocabularyCount *vc);
int vocabulary_count_increment(VocabularyCount *vc, int term_id, int quantity);
const int * vocabulary_count_array_get(VocabularyCount *vc);

#endif
