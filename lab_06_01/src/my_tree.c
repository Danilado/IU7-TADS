#include "my_tree.h"

#ifndef max
#define max(a, b) ((a > b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) ((a < b) ? (a) : (b))
#endif

struct tree_node
{
    void *data;
    size_t count;
    struct tree_node *parent;
    struct tree_node *left;
    struct tree_node *right;
};

struct tree
{
    tree_node_t root;
    compataror_t cmp;
    const char *format;
};

tree_node_t tree_node_create(void *data)
{
    tree_node_t res = calloc(1, sizeof(struct tree_node));
    if (!res)
        return NULL;

    res->parent = NULL;
    res->left = NULL;
    res->right = NULL;
    res->count = 1;
    res->data = data;

    return res;
}

void tree_node_destroy(tree_node_t *node)
{
    if (!node || !*node)
        return;

    free((*node)->data);
    free(*node);
    *node = NULL;
}

tree_t tree_create(compataror_t cmp, char *el_format)
{
    if (!cmp || !el_format)
        return NULL;

    tree_t res = calloc(1, sizeof(struct tree));
    res->cmp = cmp;
    char *buf = calloc(3 + strlen(el_format), sizeof(char));
    strcat(buf, "%");
    strcat(buf, el_format);
    strcat(buf, "\n");
    res->format = buf;
    res->root = NULL;

    return res;
}

tree_t tree_create_from_file(FILE *stream, compataror_t cmp, char *el_format)
{
    tree_t res = tree_create(cmp, el_format);
    if (!res)
        return NULL;

    size_t el_count = 0;

    fseek(stream, 0, SEEK_SET);
    int tmp;
    while (!feof(stream) && (fscanf(stream, res->format, &tmp) == 1))
    {
        int *buf = calloc(1, sizeof(int));
        *buf = tmp;

        int rc = tree_push(res, buf);
        if (rc)
        {
#ifndef NDEBUG
            printf(
            "В процессе создания дерево возникла ошибка с нехваткой памяти!\n");
#endif
            free(buf);
            break;
        }
        ++el_count;
    }

    fseek(stream, 0, SEEK_SET);

#ifndef NDEBUG
    printf("Всего считано %zu чисел\n", el_count);
#endif
    return res;
}

void tree_rec_destroy(tree_node_t node)
{
    if (!node)
        return;

    tree_rec_destroy(node->left);
    tree_rec_destroy(node->right);

    tree_node_destroy(&node);
}

void tree_destroy(tree_t *tree)
{
    if (!tree || !*tree)
        return;

    tree_rec_destroy((*tree)->root);

    free((void *)(*tree)->format);
    free(*tree);
    *tree = NULL;
}

int tree_push(tree_t tree, void *data)
{
    tree_node_t node = tree_node_create(data);
    if (!node)
        return TREE_NO_MEMORY;

    if (!tree->root)
    {
        tree->root = node;
        return EXIT_SUCCESS;
    }

    tree_node_t cur = tree->root;

    while (1)
    {
        int cmpres = tree->cmp(node->data, cur->data);

        if (cmpres == 0)
        {
            cur->count += 1;
            tree_node_destroy(&node);
            break;
        }
        else if (cmpres < 0)
        {
            if (!cur->left)
            {
                cur->left = node;
                node->parent = cur;
                break;
            }
            cur = cur->left;
        }
        else // if cmpres > 0
        {
            if (!cur->right)
            {
                cur->right = node;
                node->parent = cur;
                break;
            }
            cur = cur->right;
        }
    }

    return EXIT_SUCCESS;
}

void tree_print(tree_t tree);

void tree_print_level_node_count(tree_t tree);

tree_node_t tree_rec_search(tree_node_t base, compataror_t cmp, void *value)
{
    if (!base)
        return NULL;

    if (cmp(base->data, value) == 0)
        return base;

    tree_node_t res = tree_rec_search(base->left, cmp, value);
    if (!res)
        res = tree_rec_search(base->right, cmp, value);

    return res;
}

tree_node_t tree_find(tree_t tree, void *data)
{
    if (!tree)
        return NULL;

    if (!tree->root)
        return NULL;

    return tree_rec_search(tree->root, tree->cmp, data);
}

void node_rec_print(tree_node_t node, const char *spec)
{
    if (!node)
        return;

    node_rec_print(node->left, spec);
    node_rec_print(node->right, spec);
    printf(spec, node->data);
}

void tree_print_post_order(tree_t tree)
{
    if (tree == NULL)
        return;

    node_rec_print(tree->root, tree->format);
}

tree_node_t tree_left_most(tree_node_t cur_root)
{
    if (!cur_root->left)
        return cur_root;
    return tree_left_most(cur_root->left);
}

tree_node_t tree_right_most(tree_node_t cur_root)
{
    if (!cur_root->right)
        return cur_root;
    return tree_right_most(cur_root->right);
}

tree_node_t tree_rec_node_del(tree_node_t cur_root, tree_node_t target)
{
    if (target->data < cur_root->data)
    {
        cur_root->left = tree_rec_node_del(cur_root->left, target);
    }
    else if (target->data > cur_root->data)
    {

        cur_root->right = tree_rec_node_del(cur_root->right, target);
    }
    else
    {
        if (!cur_root->left && !cur_root->right)
        {
            if (cur_root->parent)
            {
                if (cur_root == cur_root->parent->left)
                    cur_root->parent->left = NULL;
                else
                    cur_root->parent->right = NULL;
            }
            tree_node_destroy(&cur_root);
            cur_root = NULL;
        }
        else if (!cur_root->left)
        {
            tree_node_t tmp = cur_root;
            cur_root = cur_root->right;
            cur_root->parent = tmp->parent;
            tree_node_destroy(&tmp);
        }
        else if (!cur_root->right)
        {
            tree_node_t tmp = cur_root;
            cur_root = cur_root->left;
            cur_root->parent = tmp->parent;
            tree_node_destroy(&tmp);
        }
        else
        {
            tree_node_t tmp = tree_left_most(cur_root->right);
            cur_root->data = tmp->data;
            cur_root->count = tmp->count;
            cur_root->right = tree_rec_node_del(cur_root->right, tmp);
        }
    }
    return cur_root;
}

void tree_node_delete(tree_t tree, void *value)
{
    if (!tree)
        return;

    if (!tree->root)
        return;

    tree_node_t root = tree->root;

    tree_node_t target = tree_rec_search(root, tree->cmp, value);
    if (!target)
        return;

    tree->root = tree_rec_node_del(root, target);
}

void tree_rec_apply(tree_t tree, tree_node_t cur_root,
void (*func)(tree_t, tree_node_t, void *), void *arg)
{
    if (!cur_root)
        return;

    func(tree, cur_root, arg);
    tree_rec_apply(tree, cur_root->left, func, arg);
    tree_rec_apply(tree, cur_root->right, func, arg);
}

void tree_apply(
tree_t tree, void (*func)(tree_t, tree_node_t, void *), void *arg)
{
    if (!tree || !tree->root)
        return;

    tree_rec_apply(tree, tree->root, func, arg);
}

void node_to_dot(tree_t tree, tree_node_t cur_root, void *stream)
{
    FILE *f = stream;

    if (cur_root->left)
    {
        fprintf(f, tree->format, *(int *)cur_root->data);
        fprintf(f, " -> ");
        fprintf(f, tree->format, *(int *)cur_root->left->data);
        fprintf(f, ";\n");
    }

    if (cur_root->right)
    {
        fprintf(f, tree->format, *(int *)cur_root->data);
        fprintf(f, " -> ");
        fprintf(f, tree->format, *(int *)cur_root->right->data);
        fprintf(f, ";\n");
    }
}

void tree_to_dot(FILE *f, const char *tree_name, tree_t tree)
{
    fprintf(f, "digraph %s {\n\
    node [shape=circle, fontname=\"Arial\", fontsize=12];\
    edge [fontsize=10];",
    tree_name);

    tree_apply(tree, node_to_dot, f);

    fprintf(f, "}\n");
}

int open_tree_img(char *filename, tree_t tree)
{
    char *path = calloc(strlen(filename) + 30, sizeof(char));
    if (!path)
        return TREE_NO_MEMORY;

    sprintf(path, "./img/%s.dot", filename);

    FILE *out_img = fopen(path, "w");
    if (!out_img)
        return TREE_BAD_DOT;
    tree_to_dot(out_img, "test", tree);
    fclose(out_img);

    char *command = calloc((strlen(filename) * 2 + 100), sizeof(char));
    if (!command)
        return TREE_NO_MEMORY;

    sprintf(command,
    "dot -Tpng %s -o ./img/output_%s.png && sxiv ./img/output_%s.png", path,
    filename, filename);

    system(command);

    free(path);
    free(command);

    return EXIT_SUCCESS;
}

size_t rec_get_depth(tree_node_t node, size_t cur_depth)
{
    if (!node)
        return cur_depth;

    return max(rec_get_depth(node->left, cur_depth + 1),
    rec_get_depth(node->right, cur_depth + 1));
}

size_t tree_get_depth(tree_t tree)
{
    if (!tree || !tree->root)
        return 0;

    return max(
    rec_get_depth(tree->root->left, 1), rec_get_depth(tree->root->right, 1));
}

void rec_depth_count(tree_node_t node, size_t cur_depth, size_t *arr)
{
    if (!node)
        return;

    arr[cur_depth]++;
    rec_depth_count(node->left, cur_depth + 1, arr);
    rec_depth_count(node->right, cur_depth + 1, arr);
}

void tree_print_node_level_count(tree_t tree)
{
    if (!tree || !tree->root)
    {
        printf("Дерево пустое!\n");
        return;
    }

    size_t tree_depth = tree_get_depth(tree);
    size_t count[tree_depth];

    for (size_t i = 0; i < tree_depth; ++i)
        count[i] = 0;

    rec_depth_count(tree->root, 0, count);

    for (size_t i = 0; i < tree_depth; ++i)
        printf("На слое %zu : %zu\n", i, count[i]);
}

int int_tree_min(tree_t tree)
{
    if (!tree || !tree->root)
        return 0;
    return *(int *)tree_left_most(tree->root)->data;
}

int int_tree_max(tree_t tree)
{
    if (!tree || !tree->root)
        return 0;
    return *(int *)tree_right_most(tree->root)->data;
}

#undef max
#undef min
