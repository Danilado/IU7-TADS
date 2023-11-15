#include "my_list.h"

size_t list_len(const my_list_t *list)
{
    my_list_node_t *node = list->head;
    if (!node)
        return 0;

    size_t depth = 0;

    while (node->next)
    {
        node = node->next;
        ++depth;
    }

    return depth;
}

my_list_node_t *list_node_create(int value)
{
    my_list_node_t *tmp = calloc(1, sizeof(my_list_node_t));
    if (!tmp)
        return NULL;

    tmp->value = value;
    tmp->next = NULL;
    return tmp;
}

my_list_node_t *list_get_last(const my_list_t *list)
{
    my_list_node_t *node = list->head;
    if (!node)
        return NULL;

    while (node->next)
        node = node->next;

    return node;
}

int list_push(my_list_t *list, int value)
{
    my_list_node_t *tmp = list_node_create(value);
    if (!tmp)
        return L_NOT_ENOUGH_MEMORY;

    if (!list->head)
    {
        my_list_node_t *tmp = list_node_create(value);
        if (!tmp)
            return L_NOT_ENOUGH_MEMORY;

        list->head = tmp;
    }
    else
        list_get_last(list)->next = tmp;

    return EXIT_SUCCESS;
}

int list_unshift(my_list_t *list, int value)
{
    my_list_node_t *tmp = list_node_create(value);
    if (!tmp)
        return L_NOT_ENOUGH_MEMORY;

    tmp->next = list->head;
    list->head = tmp;

    return EXIT_SUCCESS;
}

my_list_node_t *list_shift(my_list_t *list)
{
    if (!list->head)
        return NULL;
    my_list_node_t *tmp = list->head;
    list->head = list->head->next;

    free(tmp);
    return tmp;
}

void list_apply(my_list_t *list, void (*func)(my_list_node_t *))
{
    my_list_node_t *tmp = list->head;
    while (tmp)
    {
        func(tmp);
        tmp = tmp->next;
    }
}

void node_print(my_list_node_t *node)
{
    printf("%d ", node->value);
}

void list_print(const my_list_t *list)
{
    if (!list->head)
    {
        printf("Список пуст");
        return;
    }
    list_apply((my_list_t *)list, node_print);
}

void list_init(my_list_t *list)
{
    list->head = NULL;
}

void list_destroy(my_list_t *list, my_list_node_t **graveyard)
{
    my_list_node_t *node = list->head, *tmp;
    size_t i = 0;
    while (node)
    {
        tmp = node->next;
        if (graveyard)
        {
            graveyard[i] = node;
            ++i;
        }
        free(node);
        node = tmp;
    }
}

int list_copy(const my_list_t *src, my_list_t *dst)
{
    int rc;
    my_list_node_t *node = src->head;
    while (node)
    {
        rc = list_push(dst, node->value);
        if (rc)
            return rc;
        node = node->next;
    }
    return EXIT_SUCCESS;
}
