#ifndef MY_L_STACK
#define MY_L_STACK

#include "my_list.h"

#include <stdio.h>

enum list_stack_errors
{
    L_STACK_EMPTY = 3,
};

typedef struct
{                   // просто буду пушить и
    my_list_t list; // брать только с начала
} list_stack_t;

int list_stack_init(list_stack_t *stack);
void list_stack_destroy(list_stack_t *stack, my_list_node_t **graveyard);

int list_stack_push(list_stack_t *stack, int value);
int list_stack_pop(list_stack_t *stack, int *dst);

void list_stack_print_desc_series_backwards(list_stack_t *stack, FILE *dst);

#endif
