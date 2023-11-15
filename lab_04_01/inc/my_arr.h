#ifndef MY_ARR_H
#define MY_ARR_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int pb[10000];
  size_t el_count;
} my_arr_t;

enum dyn_arr_errors {
  NOT_ENOUGH_MEMORY = 1,
};

int arr_push(my_arr_t *arr, int value);
void arr_print(my_arr_t *arr);

#endif
