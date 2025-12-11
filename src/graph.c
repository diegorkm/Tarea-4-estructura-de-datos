#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Reserva memoria para la matriz.*/
static int **allocate_matrix(size_t node_count) {
    int **matrix = calloc(node_count, sizeof(int *));
    if (!matrix) return NULL;
    
    for (size_t i = 0; i < node_count; ++i) {
        matrix[i] = malloc(node_count * sizeof(int));
        if (!matrix[i]) {
            for (size_t j = 0; j < i; ++j) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

/*Convierte letras a índices (A -> 0, B -> 1).*/
size_t graph_index_from_label(char label) {
    if (label >= 'A' && label <= 'Z') return (size_t)(label - 'A');
    if (label >= 'a' && label <= 'z') return (size_t)(label - 'a');
    return MAX_GRAPH_NODES;
}

/*Crea el grafo.*/
Graph *graph_create(size_t node_count) {
    if (node_count == 0 || node_count > MAX_GRAPH_NODES) {
        fprintf(stderr, "El grafo debe tener entre 1 y %d nodos.\n", MAX_GRAPH_NODES);
        return NULL;
    }
    Graph *graph = malloc(sizeof(Graph));
    if (!graph) { perror("malloc"); return NULL; }
    
    graph->node_count = node_count;
    graph->weights = allocate_matrix(node_count);
    
    if (!graph->weights) {
        perror("malloc");
        free(graph);
        return NULL;
    }
    graph_clear(graph);
    return graph;
}

/*Limpia el grafo (pone todo en -1).*/
void graph_clear(Graph *graph) {
    if (!graph || !graph->weights) return;
    for (size_t i = 0; i < graph->node_count; ++i) {
        for (size_t j = 0; j < graph->node_count; ++j) {
            graph->weights[i][j] = -1;
        }
        graph->weights[i][i] = 0; 
    }
}

/*Libera la memoria.*/
void graph_free(Graph **graph_ptr) {
    if (!graph_ptr || !*graph_ptr) return;
    Graph *graph = *graph_ptr;
    if (graph->weights) {
        for (size_t i = 0; i < graph->node_count; ++i) {
            free(graph->weights[i]);
        }
        free(graph->weights);
    }
    free(graph);
    *graph_ptr = NULL;
}

static bool in_bounds(const Graph *graph, size_t index) {
    return graph && index < graph->node_count;
}

int graph_weight(const Graph *graph, size_t from, size_t to) {
    if (!in_bounds(graph, from) || !in_bounds(graph, to)) return -1;
    return graph->weights[from][to];
}

/*Agrega una conexión.*/
bool graph_add_edge(Graph *graph, char from_label, char to_label, int cost) {
    if (!graph || cost < 0) return false;

    size_t from = graph_index_from_label(from_label);
    size_t to = graph_index_from_label(to_label);
    if (!in_bounds(graph, from) || !in_bounds(graph, to)) {
        fprintf(stderr, "Nodos fuera de rango: %c %c\n", from_label, to_label);
        return false;
    }
    graph->weights[from][to] = cost;
    graph->weights[to][from] = cost;
    return true;
}

/*Verifica conectividad usando BFS (Búsqueda en Anchura).*/
bool graph_is_connected(const Graph *graph) {
    if (!graph || graph->node_count == 0) return false;
    
    bool visited[MAX_GRAPH_NODES] = {false};
    size_t queue[MAX_GRAPH_NODES];
    size_t head = 0, tail = 0;
    
    visited[0] = true;
    queue[tail++] = 0;

    while (head < tail) {
        size_t node = queue[head++];
        for (size_t neighbor = 0; neighbor < graph->node_count; ++neighbor) {
            if (graph->weights[node][neighbor] >= 0 && !visited[neighbor]) {
                visited[neighbor] = true;
                queue[tail++] = neighbor;
            }
        }
    }

    for (size_t i = 0; i < graph->node_count; ++i) {
        if (!visited[i]) return false;
    }
    return true;
}

void graph_print(const Graph *graph) {
    if (!graph) {
        puts("No existe un grafo inicializado.");
        return;
    }
    printf("Grafo con %zu nodos (A-%c)\n", graph->node_count, (char)('A' + graph->node_count - 1));
    for (size_t i = 0; i < graph->node_count; ++i) {
        printf("%c:", (char)('A' + i));
        for (size_t j = 0; j < graph->node_count; ++j) {
            if (graph->weights[i][j] >= 0 && i != j) {
                printf(" %c(%d)", (char)('A' + j), graph->weights[i][j]);
            }
        }
        putchar('\n');
    }
}