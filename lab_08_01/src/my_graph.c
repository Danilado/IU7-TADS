#include "my_graph.h"

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
            result->node_count = i;
            graph_destroy((&result));
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

    fprintf(f, "graph {\n\
    node [shape=circle, fontname=\"Arial\", fontsize=12];\
    edge [fontsize=10];");

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

        fprintf(f, "%zu [color=\"%s\"];\n", i, color);

        for (size_t ii = 0; ii < graph->nodes[i]->neighbour_count; ++i)
            fprintf(
            f, "%zu -> %zu;\n", i, graph->nodes[i]->neighbours[ii]->index);
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
        return EXIT_SUCCESS;
    }

    *dst = calloc(*count, sizeof(int));

    size_t tmp = 0;
    for (size_t i = 0; i < graph->node_count; ++i)
        if (!graph->nodes[i]->visited)
            (*dst)[tmp++] = i;

    return EXIT_SUCCESS;
}

int graph_show(const graph_t graph);

int graph_show_w_inaccessible_from(
const graph_t graph, size_t from, graph_node_t **dst);
