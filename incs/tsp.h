#ifndef TSP_H
#define TSP_H

#include <stdbool.h>
#include "graph.h"

typedef struct {
    bool exists;
    int cost;
    size_t order[MAX_GRAPH_NODES];
    size_t count;
} HamiltonianResult;

HamiltonianResult tsp_find_cycle(const Graph *graph);

#endif
