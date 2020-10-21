#include <stdio.h>
#include <stdlib.h>
#include "lib/sqlite/sqlite3.h"

sqlite3 * open_db(char *name)
{
  sqlite3 *db;
  int rc = sqlite3_open(name, &db);
  
  if (rc != SQLITE_OK) {        
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);    
    exit(1);
  }

  return db;
}

void prepare_statement(sqlite3 *db, char *sql, sqlite3_stmt **statement) {
  if (*statement == NULL) {
    int rc = sqlite3_prepare_v3(db, sql, -1, 0, statement, NULL);
    if (rc) {
      fprintf(stderr, "Couldn't prepare sql statement: %s\n", sqlite3_errmsg(db));
      sqlite3_close_v2(db);
      exit(1);
    }
  } else {
    sqlite3_reset(*statement);
  }
}
