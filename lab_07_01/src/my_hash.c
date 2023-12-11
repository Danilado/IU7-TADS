#include "my_hash.h"

static size_t hash_get_index(int num, int m)
{
    return (size_t)((num % m + m) % m);
}

static int is_prime(int num)
{
    if (num <= 1)
        return 0;

    for (int i = 2; i * i <= num; ++i)
        if (num % i == 0)
            return 0;

    return 1;
}

static int next_prime(int num)
{
    int res = num + 1;

    while (!is_prime(res))
        ++res;

    return res;
}

struct open_hash
{
    node_t **data;
    int datalen;
};

open_hash_t open_hash_init(int datalen)
{
    open_hash_t res = calloc(1, sizeof(struct open_hash));
    if (!res)
        return NULL;

    res->datalen = next_prime(datalen);
    res->data = calloc(res->datalen, sizeof(node_t *));
    if (!res->data)
    {
        free(res);
        return NULL;
    }

    return res;
}

void open_hash_destroy(open_hash_t *hasht)
{
    if (!hasht || !*hasht)
        return;

    for (size_t i = 0; i < (*hasht)->datalen; ++i)
        list_destroy(&((*hasht)->data[i]));

    free((*hasht)->data);
    free(*hasht);
    *hasht = NULL;
}

int open_hash_get(open_hash_t hasht, int key)
{
    if (!hasht)
        return 0;

    size_t index = hash_get_index(key, hasht->datalen);

    node_t *cell = hasht->data[index];
    if (!cell)
        return 0;

    while (cell)
    {
        if (cell->data == key)
            return cell->data;

        cell = cell->next;
    }

    return 0;
}

int open_hash_push(open_hash_t *hasht, int value)
{
    if (!hasht || !*hasht)
        return 1;

    if (open_hash_get(*hasht, value))
        return 0;

    size_t index = hash_get_index(value, (*hasht)->datalen);

    node_t *cell = (*hasht)->data[index];
    if (list_get_length(cell) >= MAX_COLLISIONS)
    {
        open_hash_restruct(hasht);
        return open_hash_push(hasht, value);
    }

    list_push_value(&((*hasht)->data[index]), value);
    return 0;
}

int open_hash_remove(open_hash_t hasht, int value)
{
    if (!open_hash_get(hasht, value))
        return 0;

    size_t index = hash_get_index(value, hasht->datalen);

    node_t *cell = hasht->data[index];
    size_t i = 0;
    while (cell)
    {
        if (cell->data == value)
            break;
        ++i;
    }

    list_remove(&(hasht->data[index]), i);

    return 0;
}

int open_hash_restruct(open_hash_t *hasht)
{
    int newsize = next_prime((*hasht)->datalen);

#ifndef NDEBUG
    printf("Реструктуризирую открытую хэш таблицу %d -> %d\n",
    (*hasht)->datalen, newsize);
#endif

    open_hash_t newhash = open_hash_init(newsize - 1);
    if (!newhash)
        return 1;

    for (size_t i = 0; i < (*hasht)->datalen; ++i)
    {
        node_t *cur = (*hasht)->data[i];
        while (cur)
        {
            open_hash_push(&newhash, cur->data);
            cur = cur->next;
        }
        list_destroy(&cur);
    }

    open_hash_destroy(hasht);
    *hasht = newhash;

    return 0;
}

void open_hash_print(open_hash_t hasht)
{
    if (!hasht)
    {
        puts("Таблица пустая!");
        return;
    }

    printf("Количество ячеек: %d\n", hasht->datalen);

    for (size_t i = 0; i < hasht->datalen; ++i)
        if (hasht->data[i])
        {
            printf("%6zu: ", i);
            node_t *cur = hasht->data[i];
            printf("%d", cur->data);
            cur = cur->next;
            while (cur)
            {
                printf(", %d", cur->data);
                cur = cur->next;
            }
            printf("\n");
        }
}

struct closed_hash
{
    int *data;
    int datalen;
};

closed_hash_t closed_hash_init(int datalen)
{
    closed_hash_t res = calloc(1, sizeof(struct closed_hash));
    if (!res)
        return NULL;

    res->datalen = next_prime(datalen);
    res->data = calloc(res->datalen, sizeof(int));
    if (!res->data)
    {
        free(res);
        return NULL;
    }

    return res;
}

void closed_hash_destroy(closed_hash_t *hasht)
{
    if (!hasht || !*hasht)
        return;

    free((*hasht)->data);
    free(*hasht);
    *hasht = NULL;
}

int *closed_hash_get(closed_hash_t hasht, int key)
{
    if (!hasht)
        return NULL;

    size_t index = hash_get_index(key, hasht->datalen);
    for (size_t i = 0; i < MAX_COLLISIONS; ++i)
        if (hasht->data[((index + i * i) % (hasht->datalen))] == key)
            return &(hasht->data[((index + i * i) % (hasht->datalen))]);

    return NULL;
}

int closed_hash_push(closed_hash_t *hasht, int value)
{
    if (!value)
        return 0;

    if (closed_hash_get(*hasht, value))
        return 0;

    size_t index = hash_get_index(value, (*hasht)->datalen);
    for (size_t i = 0; (i < MAX_COLLISIONS); ++i)
    {
        if ((*hasht)->data[((index + i * i) % ((*hasht)->datalen))] == 0)
        {
            (*hasht)->data[((index + i * i) % ((*hasht)->datalen))] = value;
            return 0;
        }
    }

    closed_hash_restruct(hasht);
    return closed_hash_push(hasht, value);
}

int closed_hash_remove(closed_hash_t hasht, int key)
{
    if (!key)
        return 0;
    if (!closed_hash_get(hasht, key))
        return 0;

    size_t index = hash_get_index(key, hasht->datalen);

    for (size_t i = 0; (i < MAX_COLLISIONS) &&
                       (((index + i * i) % (hasht->datalen)) >= hasht->datalen);
         ++i)
        if (hasht->data[((index + i * i) % (hasht->datalen))] == key)
        {
            hasht->data[((index + i * i) % (hasht->datalen))] = 0;
            return 0;
        }

    return 0;
}

int closed_hash_restruct(closed_hash_t *hasht)
{
    int newsize = next_prime((*hasht)->datalen);

#ifndef NDEBUG
    printf("Реструктуризирую закрытую хэш таблицу %d -> %d\n",
    (*hasht)->datalen, newsize);
#endif

    closed_hash_t newhash = closed_hash_init(newsize - 1);
    if (!newhash)
        return 1;

    for (size_t i = 0; i < (*hasht)->datalen; ++i)
        if ((*hasht)->data[i])
            closed_hash_push(&newhash, (*hasht)->data[i]);

    closed_hash_destroy(hasht);
    *hasht = newhash;
    return 0;
}

void closed_hash_print(closed_hash_t hasht)
{
    if (!hasht)
    {
        puts("Таблица пустая!");
        return;
    }

    printf("Количество ячеек: %d\n", hasht->datalen);

    for (size_t i = 0; i < hasht->datalen; ++i)
        if (hasht->data[i] || i == 0)
        {
            printf("%6zu: ", i);
            printf("%d\n", hasht->data[i]);
        }
}

int open_hash_fread(open_hash_t *hasht, FILE *stream)
{
    size_t el_count = 0;
    fseek(stream, 0, SEEK_SET);
    int tmp;
    while (!feof(stream) && (fscanf(stream, "%d", &tmp) == 1))
    {
        open_hash_push(hasht, tmp);
        ++el_count;
    }

    fseek(stream, 0, SEEK_SET);

#ifndef NDEBUG
    printf("Всего считано %zu чисел\n", el_count);
#endif

    return 0;
}

int closed_hash_fread(closed_hash_t *hasht, FILE *stream)
{
    size_t el_count = 0;
    fseek(stream, 0, SEEK_SET);
    int tmp;
    while (!feof(stream) && (fscanf(stream, "%d", &tmp) == 1))
    {
        closed_hash_push(hasht, tmp);
        ++el_count;
    }

    fseek(stream, 0, SEEK_SET);

#ifndef NDEBUG
    printf("Всего считано %zu чисел\n", el_count);
#endif

    return 0;
}

size_t open_hash_get_comps(open_hash_t hasht, int key)
{
    if (!hasht)
        return 0;

    size_t res = 0;
    size_t index = hash_get_index(key, hasht->datalen);

    node_t *cell = hasht->data[index];
    if (!cell)
        return 0;

    while (cell)
    {
        ++res;
        if (cell->data == key)
            return res;

        cell = cell->next;
    }

    return res;
}

size_t closed_hash_get_comps(closed_hash_t hasht, int key)
{
    size_t res = 0;
    size_t index = hash_get_index(key, hasht->datalen);
    for (size_t i = 0; i < MAX_COLLISIONS; ++i)
    {
        ++res;
        if (hasht->data[((index + i * i) % (hasht->datalen))] == key)
            return res;
    }

    return res;
}

size_t open_hash_get_size(open_hash_t hasht)
{
    if (!hasht)
        return 0;

    size_t res = 0;
    res += sizeof(hasht);
    res += sizeof(*hasht);
    res += hasht->datalen * sizeof(node_t *);

    for (size_t i = 0; i < hasht->datalen; ++i)
        if (hasht->data[i])
            res += list_get_length(hasht->data[i]) * sizeof(node_t);

    return res;
}

size_t closed_hash_get_size(closed_hash_t hasht)
{
    if (!hasht)
        return 0;

    size_t res = 0;

    res += sizeof(hasht);
    res += sizeof(*hasht);
    res += hasht->datalen * sizeof(int);

    return res;
}
