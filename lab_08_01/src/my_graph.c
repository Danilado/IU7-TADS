#include "my_graph.h"
#ifndef NDEBUG
#include <assert.h>
#endif
struct graph
{
    size_t node_count;
    graph_node_t *nodes;
};

struct graph_node
{
    size_t index;
    bool visited;
    size_t neighbour_count;
    graph_node_t *neighbours;
};

graph_t graph_create(size_t node_count)
{
    graph_t result = calloc(1, sizeof(struct graph));
    if (!result)
        return NULL;

    result->node_count = node_count;
    result->nodes = calloc(node_count, sizeof(graph_node_t));

    if (!result->nodes)
    {
        graph_destroy(&result);
        return NULL;
    }

    for (size_t i = 0; i < node_count; ++i)
    {
        graph_node_t tmp = graph_node_create(i);
        if (!tmp)
        {
            graph_destroy(&result);
            return NULL;
        }
        result->nodes[i] = tmp;
    }

    return result;
}

void graph_destroy(graph_t *graph)
{
    if (!graph | !*graph)
        return;

    if ((*graph)->nodes)
        for (size_t i = 0; i < (*graph)->node_count; ++i)
            graph_node_destroy(&((*graph)->nodes[i]));

    free((*graph)->nodes);
    free(*graph);
    *graph = NULL;
}

int graph_build_path(graph_t graph, size_t from, size_t to)
{
    if (!graph)
        return GRAPH_UNINITIALISED;

    size_t nc = graph->node_count;
    if (from >= nc || to >= nc)
        return GRAPH_OUT_OF_BOUNDS;

    graph_node_t src = graph->nodes[from];
    graph_node_t dst = graph->nodes[to];

#ifndef NDEBUG
    assert(src != NULL);
    assert(dst != NULL);
#endif

    return graph_node_add_neighbour(src, dst);
}

static void graph_clear_visited(graph_t graph)
{
    if (!graph)
        return;

    for (size_t i = 0; i < graph->node_count; ++i)
        graph->nodes[i]->visited = false;
}

static void rec_dfs(
graph_node_t node, void (*func)(graph_node_t, void *), void *param)
{
    if (!node)
        return;

    if (node->visited)
        return;

    node->visited = true;
    if (func)
        func(node, param);

    for (size_t i = 0; i < node->neighbour_count; ++i)
        rec_dfs(node->neighbours[i], func, param);
}

static void graph_dfs(
graph_t graph, size_t from, void (*func)(graph_node_t, void *), void *param)
{
    if (!graph)
        return;

    if (from >= graph->node_count)
        return;

    graph_node_t start = graph->nodes[from];

    rec_dfs(start, func, param);
}

graph_node_t graph_node_create(size_t index)
{
    graph_node_t result = calloc(1, sizeof(struct graph_node));
    if (!result)
        return NULL;

    result->index = index;
    result->visited = false;
    result->neighbour_count = 0;
    result->neighbours = NULL;

    return result;
}

void graph_node_destroy(graph_node_t *node)
{
    if (!node || !*node)
        return;

    free((*node)->neighbours);
    free(*node);
    *node = NULL;
}

int graph_node_add_neighbour(graph_node_t node, const graph_node_t neighbour)
{
    if (!node || !neighbour)
        return GRAPH_UNINITIALISED;

    graph_node_t *nbs = realloc(
    node->neighbours, sizeof(graph_node_t) * (node->neighbour_count + 1));
    if (!nbs)
        return GRAPH_NO_MEMORY;

    node->neighbours = nbs;
    node->neighbours[node->neighbour_count] = neighbour;
    ++node->neighbour_count;

    return EXIT_SUCCESS;
}

static int graph_to_dot(const graph_t graph, char *filename, bool colored)
{
    FILE *f = fopen(filename, "w");
    if (!f)
        return GRAPH_MISSING_PERMISSIONS;

    fprintf(f, "digraph {\n"
               "\tnode [shape=circle, fontname=\"Arial\", fontsize=12];\n"
               "\tedge [fontsize=10];\n");

    char *color;

    for (size_t i = 0; i < graph->node_count; ++i)
    {
        color = "black";
        if (colored)
        {
            if (graph->nodes[i]->visited)
                color = "green";
            else
                color = "red";
        }

        fprintf(f, "\t%zu [color=\"%s\"];\n", i, color);
        fprintf(f, "\t%zu -> {", i);

        for (size_t j = 0; j < graph->nodes[i]->neighbour_count; ++j)
            fprintf(f, "%zu ", graph->nodes[i]->neighbours[j]->index);

        fprintf(f, "};\n");
    }

    fprintf(f, "}\n");
    fclose(f);
    return EXIT_SUCCESS;
}

int graph_get_inaccesible_from(
const graph_t graph, size_t from, int **dst, size_t *count)
{
    if (!graph)
        return GRAPH_UNINITIALISED;

    if (from >= graph->node_count)
        return GRAPH_OUT_OF_BOUNDS;

    graph_dfs(graph, from, NULL, NULL);

    *count = 0;
    for (size_t i = 0; i < graph->node_count; ++i)
        if (!graph->nodes[i]->visited)
            ++*count;

    if (!*count)
    {
        *dst = NULL;
        graph_clear_visited(graph);
        return EXIT_SUCCESS;
    }

    *dst = calloc(*count, sizeof(int));

    size_t tmp = 0;
    for (size_t i = 0; i < graph->node_count; ++i)
        if (!graph->nodes[i]->visited)
            (*dst)[tmp++] = i;

    graph_clear_visited(graph);
    return EXIT_SUCCESS;
}

int graph_show(const graph_t graph)
{
    if (!graph)
        return GRAPH_UNINITIALISED;

    int rc = graph_to_dot(graph, "./out/graph.dot", false);
    if (rc)
        return rc;

    system("dot -Tsvg ./out/graph.dot -o ./img/output_graph.svg && sxiv "
           "./img/output_graph.svg");

    return EXIT_SUCCESS;
}

int graph_show_w_inaccessible_from(const graph_t graph, size_t from)
{
    if (!graph)
        return GRAPH_UNINITIALISED;

    if (from >= graph->node_count)
        return GRAPH_OUT_OF_BOUNDS;

    graph_dfs(graph, from, NULL, NULL);

    int rc = graph_to_dot(graph, "./out/graph.dot", true);
    graph_clear_visited(graph);
    if (rc)
        return rc;

    system("dot -Tsvg ./out/graph.dot -o ./img/output_graph.svg && sxiv "
           "./img/output_graph.svg");

    return EXIT_SUCCESS;
}

int graph_add_nodes(graph_t graph, size_t node_count)
{
    if (!node_count)
        return EXIT_SUCCESS;

    graph_node_t *tmp = realloc(
    graph->nodes, sizeof(graph_node_t) * (graph->node_count + node_count));
    if (!tmp)
        return GRAPH_NO_MEMORY;

    graph->nodes = tmp;

    for (size_t i = graph->node_count; i < graph->node_count + node_count; ++i)
    {
        graph_node_t tmpnode = graph_node_create(i);
        if (!tmpnode)
            return GRAPH_NO_MEMORY;
        graph->nodes[i] = tmpnode;
    }

    graph->node_count += node_count;
    return EXIT_SUCCESS;
}

graph_t graph_create_from_file(FILE *src)
{
    if (!src)
        return NULL;

    size_t node_count;
    size_t path_count = 0;
    if (fscanf(src, "%zu", &node_count) != 1)
        return NULL;

    graph_t res = graph_create(node_count);
    if (!res)
        return NULL;

    size_t tmp1, tmp2;
    while (fscanf(src, "%zu%zu", &tmp1, &tmp2) == 2)
    {
        ++path_count;
        graph_build_path(res, tmp1, tmp2);
    }

#ifndef NDEBUG
    printf("Read from file:\n%zu nodes; %zu paths\n\n", node_count, path_count);
#endif

    return res;
}

void graph_write_to_file(const graph_t graph, FILE *dst)
{
    if (!dst)
        return;

    fprintf(dst, "%zu\n", graph->node_count);
    for (size_t i = 0; i < graph->node_count; ++i)
    {
        graph_node_t cur = graph->nodes[i];
        for (size_t j = 0; j < cur->neighbour_count; ++j)
            fprintf(dst, "%zu %zu\n", i, cur->neighbours[j]->index);
    }

    return;
}
