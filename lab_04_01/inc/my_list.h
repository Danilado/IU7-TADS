#ifndef MY_LIST_H
#define MY_LIST_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

enum list_errors
{
    L_NOT_ENOUGH_MEMORY = 1,
};

typedef struct my_list_node
{
    int value;
    struct my_list_node *next;
} my_list_node_t;

my_list_node_t *list_node_create(int value);

typedef struct
{
    my_list_node_t *head;
} my_list_t;

int list_push(my_list_t *list, int value);

int list_unshift(my_list_t *list, int value);
my_list_node_t *list_shift(my_list_t *list);

my_list_node_t *list_get_last(const my_list_t *list);
size_t list_len(const my_list_t *list);

void list_apply(my_list_t *list, void (*func)(my_list_node_t *));
void list_print(const my_list_t *list);

void list_init(my_list_t *list);
void list_destroy(my_list_t *list, my_list_node_t **graveyard);

int list_copy(const my_list_t *src, my_list_t *dst);

#endif
