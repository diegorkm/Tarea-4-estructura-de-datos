#include "tsp.h"
#include <limits.h>
#include <stddef.h>

/*Función recursiva (Backtracking) para encontrar el ciclo.*/
static void search_cycle(const Graph *graph,
                         size_t current,
                         size_t depth,
                         int current_cost,
                         HamiltonianResult *best,
                         bool visited[MAX_GRAPH_NODES],
                         size_t path[MAX_GRAPH_NODES]) {
    if (!graph) return;

    if (depth == graph->node_count) {
        int return_cost = graph_weight(graph, current, 0);
        if (return_cost < 0) return; 
        
        int total_cost = current_cost + return_cost;

        if (!best->exists || total_cost < best->cost) {
            best->exists = true;
            best->cost = total_cost;
            best->count = graph->node_count + 1;
            for (size_t i = 0; i < graph->node_count; ++i) {
                best->order[i] = path[i];
            }
            best->order[graph->node_count] = 0; 
        }
        return;
    }

    for (size_t next = 1; next < graph->node_count; ++next) {
        if (visited[next]) continue; 

        int edge_cost = graph_weight(graph, current, next);
        if (edge_cost < 0) continue; 
        
        int projected_cost = current_cost + edge_cost;
        
        if (best->exists && projected_cost >= best->cost) {
            continue;
        }

        visited[next] = true;
        path[depth] = next;
        
        search_cycle(graph, next, depth + 1, projected_cost, best, visited, path);
        
        visited[next] = false;
    }
}

/*Inicia la búsqueda del Ciclo Hamiltoniano (TSP).*/
HamiltonianResult tsp_find_cycle(const Graph *graph) {
    HamiltonianResult result = {.exists = false, .cost = INT_MAX, .count = 0};
    if (!graph) return result;
    
    bool visited[MAX_GRAPH_NODES] = {false};
    size_t path[MAX_GRAPH_NODES] = {0};

    visited[0] = true;
    path[0] = 0;
    
    search_cycle(graph, 0, 1, 0, &result, visited, path);
    return result;
}