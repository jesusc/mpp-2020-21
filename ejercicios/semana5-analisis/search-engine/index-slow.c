/**
 * Related links:
 *   https://sqlite.org/threadsafe.html
 *
 * About SQLite being slow:
 * - https://www.sqlite.org/faq.html#q19
 * - We need to use transactions
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

void exec_command(sqlite3 *db, char *sql)
{
  char *err_msg = 0;
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {    
    fprintf(stderr, "SQL error: %s\n", err_msg);
    
    sqlite3_free(err_msg);        
    sqlite3_close(db);
    
    exit(1);
  }
}


sqlite3 * create_db(char *name)
{
  sqlite3 *db = open_db(name);

  char *sql =
    "DROP TABLE IF EXISTS terms;"
    "DROP TABLE IF EXISTS docs;"
    "DROP TABLE IF EXISTS terms_docs;" 
    "CREATE TABLE terms(term VARCHAR(255), term_id INT, PRIMARY KEY(term));"
    "CREATE TABLE docs(doc_id INT, path TEXT, size INT);"
    "CREATE TABLE terms_docs(term_id INT, position INT, doc_id INT, PRIMARY KEY(term_id, position));"; 

  exec_command(db, sql);
  // exec_command(db, "PRAGMA synchronous=OFF;");
  
  return db;
}

#define BUFFER_SIZE 2048

/**
 * Taken from: https://lloydrochester.com/post/c/sqlite-api/
 *
 * Also see: https://stackoverflow.com/questions/55218499/cant-insert-into-a-database-using-sqlite3-bind-text 
 */ 
int insert_term(sqlite3 *db, char *term, int term_position, int doc_id, int next_id)
{
  sqlite3_stmt *p_select = NULL; 
  sqlite3_stmt *p_insert = NULL; 
  sqlite3_stmt *p_insert_term_doc = NULL; 
  
  char *select = "SELECT term_id FROM terms WHERE term = ?1;";
  char *insert_term_doc = "INSERT INTO terms_docs(term_id, position, doc_id) VALUES(?1, ?2, ?3);";
  int term_id;
  
  prepare_statement(db, select, &p_select);
  prepare_statement(db, insert_term_doc, &p_insert_term_doc);

  sqlite3_bind_text(p_select, 1, term, -1, NULL);
  if (sqlite3_step(p_select) == SQLITE_DONE) {
    char *insert = "INSERT INTO terms(term, term_id) VALUES(?1, ?2)";

    prepare_statement(db, insert, &p_insert);
    
    sqlite3_bind_text(p_insert, 1, term, -1, NULL);
    sqlite3_bind_int(p_insert, 2, next_id);    
    sqlite3_step(p_insert);

    term_id = next_id;
    next_id++;
  } else {
    term_id = sqlite3_column_int(p_select, 0);    
  }

  sqlite3_bind_int(p_insert_term_doc, 1, term_id);
  sqlite3_bind_int(p_insert_term_doc, 2, term_position);
  sqlite3_bind_int(p_insert_term_doc, 3, doc_id); 
  sqlite3_step(p_insert_term_doc);

  sqlite3_finalize(p_select);
  sqlite3_finalize(p_insert);
  sqlite3_finalize(p_insert_term_doc);
  
  return next_id;
}

#ifdef PROFILE
  static long long time_start = -1;
  static long total_words = 0;
#endif


/**
 * This function is not thread safe! 
 */
int process_file(sqlite3 *db, char *filename, int doc_id, int next_id)
{
  FILE *input_file = fopen(filename, "r");
  char buffer[BUFFER_SIZE];
  char *last_word;
  char *delimiter_characters = SEARCH_DELIMITER_CHARACTERS;

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
      // Gets each token as a string and prints it
      last_word = strtok(buffer, delimiter_characters );
      while( last_word != NULL ){
	next_id = insert_term(db, last_word, position, doc_id, next_id);
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
  }
  return next_id;
}

int main(int argc, char **argv)
{
  char * index_resources;
  sqlite3 *db;

  DIR *d;
  struct dirent *dir;

  if (argc != 3) {
    printf("Usage: ./index folder db-path\n");
    return -1;
  }

  index_resources = argv[1];
  db = create_db(argv[2]);
  
  d = opendir(index_resources);
  if (! d) {
    fprintf(stderr, "Invalid folder: %s\n", index_resources);
  }

  int count = 0;
  int next_id = 0;
  while ((dir = readdir(d)) != NULL) {
    if (dir->d_type == DT_REG) {
      char *filename = (char *) malloc(strlen(index_resources) + strlen(dir->d_name) + 2);

      strcpy(filename, index_resources);
      strcat(filename, "/");  /* DIR_SEPARATOR = / */
      strcat(filename, dir->d_name); /* Adds '\0' automatically */
	
      printf("Processing %s\n", filename);
      next_id = process_file(db, filename, count, next_id);
      count++;

      free(filename);

      /*      
#ifdef PROFILE     
      if (count == 3)
	break;
#endif
      */
    } 
  }
  
  closedir(d);
  
  sqlite3_close(db);    
  return 0;  
}
