#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include <time.h>

#include "../dependencies/dcs/dcs.h"

extern Statement execute_sql(sql_t raw_sql, Table *table);
extern Table *open_table(char *table_name);
extern void close_table(Table *table);

int generate_random_id()
{
    srand(time(NULL));
    return rand();
}

EMSCRIPTEN_KEEPALIVE
void make_clob(char *clob)
{
    Table *table = open_table("clob");
    char new_insert[264]; // 264 = COLUMN_CONTENT_SIZE + 8 (for int size) + 1 (for '\0')
    sprintf(new_insert, "INSERT %d %s", generate_random_id(), clob);
    execute_sql(new_insert, table);
    close_table(table);
}

EMSCRIPTEN_KEEPALIVE
char* get_clobs()
{
    Table *table = open_table("clob");
    sql_t new_select = SQL(SELECT);
    Statement results = execute_sql(new_select, table);
    close_table(table);
    return results.select_result;
}