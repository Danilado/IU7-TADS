#ifndef MY_ARR_STACK
#define MY_ARR_STACK

#include "my_arr.h"

#include <stdio.h>

enum stack_errors
{
    DA_STACK_EMPTY = 2,
};

typedef struct
{
    my_arr_t arr;
    int *PS;
} dynarr_stack_t;

int arr_stack_init(dynarr_stack_t *stack);
void dynarr_stack_destroy(dynarr_stack_t *stack);

int arr_stack_push(dynarr_stack_t *stack, int value);
int arr_stack_pop(dynarr_stack_t *stack, int *dst);

void arr_stack_print_desc_series_backwards(dynarr_stack_t *stack, FILE *dst);

#endif