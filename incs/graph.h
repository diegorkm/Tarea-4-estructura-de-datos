#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_GRAPH_NODES 26

typedef struct {
    size_t node_count;
    int **weights;
} Graph;

Graph *graph_create(size_t node_count);
void graph_clear(Graph *graph);
void graph_free(Graph **graph);
bool graph_add_edge(Graph *graph, char from_label, char to_label, int cost);
bool graph_is_connected(const Graph *graph);
void graph_print(const Graph *graph);
size_t graph_index_from_label(char label);
int graph_weight(const Graph *graph, size_t from, size_t to);

#endif
