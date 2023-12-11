#ifndef MY_HASH
#define MY_HASH

#include "my_list.h"
#include <math.h>
#include <stdlib.h>

#define MAX_COLLISIONS 4

typedef struct open_hash *open_hash_t;

open_hash_t open_hash_init(int datalen);
void open_hash_destroy(open_hash_t *hasht);

int open_hash_get(open_hash_t hasht, int key);
int open_hash_push(open_hash_t *hasht, int value);
int open_hash_remove(open_hash_t hasht, int key);

int open_hash_restruct(open_hash_t *hasht);

int open_hash_fread(open_hash_t *hasht, FILE *stream);
void open_hash_print(open_hash_t hasht);

typedef struct closed_hash *closed_hash_t;

closed_hash_t closed_hash_init(int datalen);
void closed_hash_destroy(closed_hash_t *hasht);

int *closed_hash_get(closed_hash_t hasht, int key);
int closed_hash_push(closed_hash_t *hasht, int value);
int closed_hash_remove(closed_hash_t hasht, int key);

int closed_hash_restruct(closed_hash_t *hasht);

int closed_hash_fread(closed_hash_t *hasht, FILE *stream);
void closed_hash_print(closed_hash_t hasht);

size_t open_hash_get_size(open_hash_t hasht);
size_t closed_hash_get_size(closed_hash_t hasht);

size_t open_hash_get_comps(open_hash_t hasht, int key);
size_t closed_hash_get_comps(closed_hash_t hasht, int key);

#endif