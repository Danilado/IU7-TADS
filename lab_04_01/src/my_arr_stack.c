#include "my_arr_stack.h"

int arr_stack_init(dynarr_stack_t *stack)
{
    stack->arr.el_count = 0;
    stack->PS = stack->arr.pb - 1;

    return EXIT_SUCCESS;
}

// void dynarr_stack_destroy(dynarr_stack_t *stack)
// {
//     dyn_arr_destroy(&stack->arr);
//     stack->PS = NULL;
// }

int arr_stack_push(dynarr_stack_t *stack, int value)
{
    ++stack->PS;
    ++stack->arr.el_count;

    *stack->PS = value;
    return EXIT_SUCCESS;
}

int arr_stack_pop(dynarr_stack_t *stack, int *dst)
{
    if (stack->PS < stack->arr.pb)
        return DA_STACK_EMPTY;

    *dst = *stack->PS;
    --stack->PS;
    --stack->arr.el_count;

    return EXIT_SUCCESS;
}

void arr_stack_print_desc_series_backwards(dynarr_stack_t *stack, FILE *dst)
{
    if (stack->PS < stack->arr.pb)
    {
        if (dst != NULL)
            fprintf(dst, "%s", "стэк пуст");
        return;
    }
    if (dst != NULL)
        fprintf(dst, "%s", "dynarr_stack series:\n");

    int rc = 0;
    int cur = 0;
    int prev = 0;

    rc = arr_stack_pop(stack, &cur);
    if (dst != NULL)
        fprintf(dst, "%d ", cur);

    while (!rc)
    {
        prev = cur;
        rc = arr_stack_pop(stack, &cur);
        if (rc)
            break;

        if (cur > prev)
        {
            if (dst != NULL)
                fprintf(dst, "%d ", cur);
        }
        else if (dst != NULL)
            fprintf(dst, "\n%d ", cur);
    }
}
