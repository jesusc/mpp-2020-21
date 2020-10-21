/**
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include "lib/sqlite/sqlite3.h"
#include "lib/vector/vector.h"
#include "utils.h"
#include "db.h"

#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>

#include <glib.h>

void
process_query(sqlite3 *db, char *query)
{
  char *delimiter_characters = SEARCH_DELIMITER_CHARACTERS;
  char *last_word;

  GHashTable *word_count = g_hash_table_new(g_str_hash, g_str_equal);

  /* Compute the pairs word - count */
  last_word = strtok(query, delimiter_characters);
  while( last_word != NULL ){
    char *key;
    gpointer p_count;
    
    if (g_hash_table_lookup_extended(word_count, last_word, NULL, &p_count)) {
      // We have key and we increment the count
      p_count = GINT_TO_POINTER(GPOINTER_TO_INT(p_count) + 1);
    } else {
      key = g_strdup(last_word);
      p_count = GINT_TO_POINTER(1);
    }

    g_hash_table_insert(word_count, key, p_count);

    last_word = strtok(NULL, delimiter_characters);
  }

  GHashTableIter iter;
  gpointer key;
  gpointer value;
  
  GHashTable *scores = g_hash_table_new(g_int_hash, g_int_equal);  
  
  g_hash_table_iter_init(&iter, word_count);

  char *select = "SELECT term_id FROM terms WHERE term = ?1;";
  sqlite3_stmt *p_select = NULL;  
  prepare_statement(db, select, &p_select);  

  char *count_docs = "SELECT COUNT(*), doc_id FROM terms_docs WHERE term_id = ?1 GROUP BY doc_id;";
  sqlite3_stmt *p_count_docs = NULL;  
  prepare_statement(db, count_docs, &p_count_docs);  

  while(g_hash_table_iter_next(&iter, &key, &value)) {
    printf("[%s] %d\n", (char *) key, GPOINTER_TO_INT(value));
    
    sqlite3_bind_text(p_select, 1, (char *) key, -1, NULL);
    if (sqlite3_step(p_select) == SQLITE_ROW) {
      int term_id = sqlite3_column_int(p_select, 0);

      sqlite3_bind_int(p_count_docs, 1, term_id);
      while (sqlite3_step(p_count_docs) == SQLITE_ROW) {
	double *score;	
	int count = sqlite3_column_int(p_count_docs, 0);
	int doc_id = sqlite3_column_int(p_count_docs, 1);
	gpointer doc_id_key;
	gpointer doc_score;
	
	printf("doc_id = %d, count = %d\n", doc_id, count);

	if (g_hash_table_lookup_extended(scores, &doc_id, &doc_id_key, &doc_score)) {
	  // We have already computed part of doc_id score
	  score = (double *) doc_score;
	} else {
	  score = (double *) malloc(sizeof(double));
	  *score = 0.0d;

	  int *doc_id_alloc = g_new0(gint, 1);
	  *doc_id_alloc = doc_id;
	  
	  doc_id_key = doc_id_alloc;
	}
	
	int word_count = GPOINTER_TO_INT(value);
	
	*score = *score + (word_count * count); 	
	// TODO: Compute log M + 1 / df(w)

	g_hash_table_insert(scores, doc_id_key, score);	
      }

      sqlite3_reset(p_count_docs);
    }

    sqlite3_reset(p_select);
  }

  // TODO: Sort this (e.g, array + merge sort)
  g_hash_table_iter_init(&iter, scores);
  while(g_hash_table_iter_next(&iter, &key, &value)) {    
    int *doc_id = (int *) key;
    double *score = (double *) value;
  
    printf("%d %f\n", *doc_id, *score);
  }

}

  

int main(int argc, char **argv)
{
  char * query_text;
  sqlite3 *db;

  if (argc != 3) {
    printf("Usage: ./query db-path query-text\n");
    return -1;
  }
 
  db = open_db(argv[1]);
  query_text = argv[2];

  printf("Processing query: [%s]\n", query_text);
  
  process_query(db, query_text);
  
  sqlite3_close(db);    
  return 0;  
}
