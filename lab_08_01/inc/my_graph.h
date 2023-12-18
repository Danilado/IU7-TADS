#ifndef MY_GRAPH_H
#define MY_GRAPH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum graph_errors
{
    GRAPH_NO_MEMORY = 1,
    GRAPH_OUT_OF_BOUNDS,
    GRAPH_MISSING_PERMISSIONS,
    GRAPH_UNINITIALISED
};

typedef struct graph *graph_t;
typedef struct graph_node *graph_node_t;

graph_t graph_create(size_t node_count);
void graph_destroy(graph_t *graph);
int graph_build_path(graph_t graph, size_t from, size_t to);

graph_node_t graph_node_create(size_t index);
void graph_node_destroy(graph_node_t *node);
int graph_node_add_neighbour(graph_node_t node, const graph_node_t neighbour);

int graph_get_inaccesible_from(
const graph_t graph, size_t from, int **dst, size_t *count);

int graph_show(const graph_t graph);
int graph_show_w_inaccessible_from(
const graph_t graph, size_t from, graph_node_t **dst);

#endif
