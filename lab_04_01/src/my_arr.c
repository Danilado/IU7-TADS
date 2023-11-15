#include "my_arr.h"

// int dyn_arr_init(size_t el_count, my_dyn_arr_t *dst)
// {
//     void *ptr = calloc(el_count, sizeof(int));
//     if (ptr == NULL)
//         return NOT_ENOUGH_MEMORY;

//     dst->pb = ptr;
//     dst->pe = (int *)ptr + sizeof(int) * el_count;
//     dst->el_count = 0;

//     return EXIT_SUCCESS;
// }

// void dyn_arr_destroy(my_dyn_arr_t *arr)
// {
//     free(arr->pb);
//     arr->pb = NULL;
//     arr->pe = NULL;
//     arr->el_count = 0;
// }

// int dyn_arr_expand(my_dyn_arr_t *arr)
// {
//     size_t new_max_el_count = (size_t)((arr->pe - arr->pb + 5) * 1.15);

//     void *tmp = realloc(arr->pb, new_max_el_count * sizeof(int));

//     if (!tmp)
//         return NOT_ENOUGH_MEMORY;

//     arr->pb = tmp;
//     arr->pe = arr->pb + new_max_el_count;

//     return EXIT_SUCCESS;
// }

int arr_push(my_arr_t *arr, int value)
{
    if (arr->el_count >= 10000)
        return NOT_ENOUGH_MEMORY;

    arr->pb[arr->el_count] = value;
    ++arr->el_count;
    return EXIT_SUCCESS;
}

// int dyn_arr_minimize(my_dyn_arr_t *arr)
// {
//     void *tmp = realloc(arr->pb, arr->el_count * sizeof(int));
//     if (!tmp)
//         return NOT_ENOUGH_MEMORY;

//     arr->pe = arr->pb + arr->el_count;
//     return EXIT_SUCCESS;
// }

void arr_print(my_arr_t *arr)
{
    if (!arr->el_count)
    {
        printf("Массив пуст");
        return;
    }

    for (size_t i = 0; i < arr->el_count; ++i)
        printf("%d ", arr->pb[i]);
}
