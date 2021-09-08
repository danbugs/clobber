/** Includes
*/
#include <stdio.h> // sprintf
#include <stdlib.h> // malloc, free, srand, rand
#include <time.h> // time
#include <emscripten.h>

/** Dependencies
*/
#include "../dependencies/dcs/dcs.h"

/** Functions from dependencies
*/
extern char* execute_sql(sql_t raw_sql, Table *table);
extern Table *open_table(char *table_name);
extern void close_table(Table *table);

/** API functions
*/
EMSCRIPTEN_KEEPALIVE
void make_clob(char *clob)
{
    Table *table = open_table("clob");
    char *new_insert;
    srand(time(NULL));
    asprintf(&new_insert, "INSERT %d %s", rand(), clob);
    execute_sql(new_insert, table);
    close_table(table);
    free(new_insert);
}

EMSCRIPTEN_KEEPALIVE
char* get_clobs()
{
    Table *table = open_table("clob");
    sql_t new_select = SQL(SELECT);
    char* results = execute_sql(new_select, table);
    close_table(table);
    return results;
}