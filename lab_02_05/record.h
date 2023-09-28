#ifndef RECORD_H
#define RECORD_H

#define MAX_NAME_LEN 20

#include "utils.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char name_t[MAX_NAME_LEN + 1];

enum musical_types
{
    BALLET = 1,
    OPERA,
    MUSICAL,
    MUSICAL_TYPE_N,
};

typedef struct
{
    unsigned char age_restriction;
    name_t composer;
    name_t country;
    unsigned char type;
    short length;
} musical_attr_t;

typedef struct
{
    int32_t min_price;
    int32_t max_price;
} price_t;

enum play_type
{
    PLAY = 1,
    DRAMA,
    COMEDY,
    FAIRYTALE,
    MUSIC,
    PLAY_TYPE_N,
};

typedef struct
{
    name_t theatre_name;
    name_t play_name;
    price_t ticket_prices;
    unsigned char play_type;
    union {
        unsigned char age_restriction;
        musical_attr_t musical_attrs;
    } play_attrs;
} record_t;

void record_init(record_t *record);

enum record_input_errors
{
    NO_THEATRE_NAME = 1,
    THEATRE_NAME_TOO_LONG,
    NO_PLAY_NAME,
    PLAY_NAME_TOO_LONG,
    PRICE_LOWER_THAN_ZERO,
    PRICE_MINMAX_MISMATCH,
    PLAY_TYPE_OUT_OF_BOUNDS,
    BAD_AGE_RESTRICTION,
    NO_COMPOSER,
    COMPOSER_TOO_LONG,
    NO_COUNTRY,
    COUNTRY_TOO_LONG,
    MUSICAL_TYPE_OUT_OF_BOUNDS,
    BAD_PLAY_LENGTH,
    STRING_FIELDS_OVERLAP,
};
int check_age(int32_t age);
int check_length(int32_t length);
int record_scan(record_t *record);
int check_prices(int32_t a, int32_t b);
int check_play_type(int32_t input);
int check_musical_type(int32_t input);
int validate_record(record_t *record);

enum scan_errors
{
    STRING_TOO_LONG = 1,
    NO_STRING,
    FGETS_ERR,
    BAD_NUMBER,
};
int get_name_t_from_buf(name_t *dst, char *buf);

void record_print(record_t *record, size_t index);
#endif
