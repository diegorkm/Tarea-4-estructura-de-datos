#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "tsp.h"

static void print_help(void) {
    puts("Comandos disponibles:");
    puts("  start <n>        Crea un grafo con n nodos (A...)");
    puts("  read <archivo>   Carga las aristas desde archivo y verifica rutas");
    puts("  graph            Muestra el grafo actual");
    puts("  help             Muestra esta ayuda");
    puts("  exit             Libera recursos y termina");
}

static char *trim(char *line) {
    if (!line) {
        return line;
    }
    while (isspace((unsigned char)*line)) {
        ++line;
    }
    if (*line == '\0') {
        return line;
    }
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) {
        *end-- = '\0';
    }
    return line;
}

static bool ensure_graph_exists(Graph *graph) {
    if (!graph) {
        puts("Debe crear el grafo primero con: start <cantidad>");
        return false;
    }
    return true;
}

static bool load_edges_from_file(Graph *graph, const char *path) {
    if (!graph) {
        return false;
    }
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("No se pudo abrir el archivo");
        return false;
    }

    graph_clear(graph);
    char from, to;
    int cost;
    int line_number = 0;
    bool ok = true;
    while (!feof(fp)) {
        int read_items = fscanf(fp, " %c %c %d", &from, &to, &cost);
        line_number++;
        if (read_items == EOF) {
            break;
        }
        if (read_items != 3) {
            fprintf(stderr, "Formato inválido en la línea %d\n", line_number);
            ok = false;
            continue;
        }
        if (!graph_add_edge(graph, from, to, cost)) {
            fprintf(stderr, "No se pudo agregar la arista %c %c %d\n", from, to, cost);
            ok = false;
        }
    }
    fclose(fp);
    return ok;
}

static void print_route(const HamiltonianResult *result) {
    if (!result || !result->exists) {
        return;
    }
    printf("Ruta a seguir: ");
    for (size_t i = 0; i < result->count; ++i) {
        printf("%c ", (char)('A' + result->order[i]));
    }
    putchar('\n');
    printf("Costo total mínimo: %d\n", result->cost);
}

int main(void) {
    Graph *graph = NULL;
    char buffer[256];

    puts("Planificador de Vendedor Viajante (pvv). Escriba help para ayuda.");
    while (1) {
        printf("pvv> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            puts("\nEntrada finalizada.");
            break;
        }

        char *line = trim(buffer);
        if (*line == '\0') {
            continue;
        }
        char *command = strtok(line, " ");
        if (!command) {
            continue;
        }

        if (strcmp(command, "start") == 0) {
            char *arg = strtok(NULL, " ");
            if (!arg) {
                puts("Uso: start <cantidad_nodos>");
                continue;
            }
            int nodes = atoi(arg);
            if (nodes <= 0 || nodes > MAX_GRAPH_NODES) {
                printf("La cantidad de nodos debe estar entre 1 y %d\n", MAX_GRAPH_NODES);
                continue;
            }
            graph_free(&graph);
            graph = graph_create((size_t)nodes);
            if (graph) {
                printf("Grafo creado con %d nodos\n", nodes);
            }
            continue;
        }

        if (strcmp(command, "read") == 0) {
            char *path = strtok(NULL, " ");
            if (!ensure_graph_exists(graph) || !path) {
                puts("Uso: read <ruta_archivo>");
                continue;
            }
            printf("Agregando enlaces desde archivo %s....\n", path);
            if (!load_edges_from_file(graph, path)) {
                puts("Ocurrieron errores al leer el archivo. Revise los mensajes anteriores.");
                continue;
            }
            puts("Verificando que existe una ruta....");
            if (!graph_is_connected(graph)) {
                puts("No existe un camino que recorra todas las ciudades y regrese a la ciudad de origen.");
                continue;
            }
            HamiltonianResult result = tsp_find_cycle(graph);
            if (!result.exists) {
                puts("No existe un camino que recorra todas las ciudades y regrese a la ciudad de origen.");
                continue;
            }
            puts("Existe un camino que recorre todas las ciudades y regresa a la ciudad de origen.");
            print_route(&result);
            continue;
        }

        if (strcmp(command, "graph") == 0) {
            if (ensure_graph_exists(graph)) {
                graph_print(graph);
            }
            continue;
        }

        if (strcmp(command, "help") == 0) {
            print_help();
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            puts("Limpiando cache y saliendo ...");
            break;
        }

        printf("Comando desconocido: %s\n", command);
        print_help();
    }

    graph_free(&graph);
    return 0;
}
