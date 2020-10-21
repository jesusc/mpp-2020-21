#ifndef DB_H
#define DB_H

sqlite3 * open_db(char *name);

void prepare_statement(sqlite3 *db, char *sql, sqlite3_stmt **statement);

#endif
