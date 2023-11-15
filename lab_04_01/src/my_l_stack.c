#include "my_l_stack.h"

int list_stack_init(list_stack_t *stack)
{
    list_init(&stack->list);
    return EXIT_SUCCESS;
}

void list_stack_destroy(list_stack_t *stack, my_list_node_t **graveyard)
{
    list_destroy(&stack->list, graveyard);
}

int list_stack_push(list_stack_t *stack, int value)
{
    return list_unshift(&stack->list, value);
}

int list_stack_pop(list_stack_t *stack, int *dst)
{
    if (!stack->list.head)
        return 1;

    *dst = stack->list.head->value;
    my_list_node_t *res = list_shift(&stack->list);

    if (!res)
        return L_STACK_EMPTY;

    return EXIT_SUCCESS;
}

void list_stack_print_desc_series_backwards(list_stack_t *stack, FILE *dst)
{
    if (!stack->list.head)
    {
        if (dst != NULL)
            fprintf(dst, "%s", "стэк пуст");
        return;
    }
    if (dst != NULL)
        fprintf(dst, "%s", "list_stack series:\n");

    int rc = 0;
    int cur = 0;
    int prev = 0;

    rc = list_stack_pop(stack, &cur);
    if (dst != NULL)
        fprintf(dst, "%d ", cur);

    while (!rc)
    {
        prev = cur;
        rc = list_stack_pop(stack, &cur);
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
