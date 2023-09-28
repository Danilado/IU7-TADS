#ifndef RECORD_TABLE_H
#define RECORD_TABLE_H

#include "record.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    size_t el_count;
    size_t reserved_el_count;
    record_t *dataptr;
} record_table_t;

enum table_errors
{
    NOT_ENOUGH_MEMORY = 1,
    BAD_FILE_SIZE,
    CANT_WRITE,
    BAD_FILENAME,
};

typedef struct
{
    size_t main_index;
    union {
        char *text;
        int32_t num;
    } value;
} key_t;

int validate_table(record_table_t *table, char *filename);
int rec_table_push(record_table_t *table, record_t *record);
int rec_table_del(record_table_t *table, size_t index);
void rec_table_print(const record_table_t *table);
void rec_table_print_keyt(const record_table_t *table, key_t *order);
int rec_table_init(record_table_t *table, size_t el_count);

int table_fscan(record_table_t *table, char *filename);
int table_fwrite(record_table_t *table, char *filename);

enum KEYS
{
    T_NAME = 1,
    MIN_PRICE,
    KEY_N,
};
int sort_table(record_table_t *table, int32_t key);
int sort_key_table(record_table_t *table, key_t *key_table, int32_t key);

int table_thname_cmp(const void *l, const void *r);
int key_thname_cmp(const void *l, const void *r);

int table_minprice_cmp(const void *l, const void *r);
int form_key_table(record_table_t *src, key_t **key_table, int32_t key);
int key_minprice_cmp(const void *l, const void *r);

int copy_table(record_table_t *src, record_table_t *dst);

int filter_table(
record_table_t *src, bool *dst, int32_t age_restr, int32_t max_length);
void rec_table_print_flagged(const record_table_t *table, bool *flags);

#endif