#ifndef MY_TREE_H
#define MY_TREE_H

// #include "defines.h"
// #include "useful_funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum tree_errors
{
    TREE_NO_MEMORY = 1,
    TREE_BAD_DOT,
};

typedef struct tree_node *tree_node_t;
typedef struct tree *tree_t;

typedef int (*compataror_t)(const void *, const void *);

tree_node_t tree_node_create(void *data);

void tree_node_destroy(tree_node_t *node);

tree_t tree_create(compataror_t cmp, char *el_format);

tree_t tree_create_from_file(FILE *stream, compataror_t cmp, char *el_format);

void tree_destroy(tree_t *tree);

int tree_push(tree_t tree, void *data);

void tree_to_dot(FILE *f, const char *header, tree_t tree);

int open_tree_img(char *filename, tree_t tree);

void tree_print_node_level_count(tree_t tree);

int int_tree_min(tree_t tree);
int int_tree_max(tree_t tree);

#endif
