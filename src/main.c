#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "tsp.h"

/*Limpia el texto que ingresa el usuario. Si escribes "  read archivo.txt  ",
esta función lo convierte en "read archivo.txt" para evitar errores.*/
static char *trim(char *line) {
    if (!line) return line;
    while (isspace((unsigned char)*line)) line++;
    if (*line == '\0') return line;
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) *end-- = '\0';
    return line;
}

/*Muestra el menú de ayuda.*/
static void print_help(void) {
    puts("Comandos disponibles:");
    puts("  start <n>        Crea un grafo con n nodos (A...)");
    puts("  read <archivo>   Carga las aristas desde archivo y verifica rutas");
    puts("  graph            Muestra el grafo actual");
    puts("  help             Muestra esta ayuda");
    puts("  clear            Limpia la pantalla");
    puts("  exit             Libera recursos y termina");
}

/*Pide memoria para el grafo. Es fundamental validar que el número sea
positivo y menor al máximo.*/
static void handle_start(char *arg, Graph **graph) {
    if (!arg) {
        puts("Uso: start <cantidad_nodos>");
        return;
    }

    for (int i = 0; arg[i] != '\0'; i++) {
        if (!isdigit((unsigned char)arg[i])) {
            puts("Error: La cantidad de nodos debe ser un numero entero.");
            return;
        }
    }

    int nodes = atoi(arg);
    if (nodes <= 0 || nodes > MAX_GRAPH_NODES) {
        printf("La cantidad de nodos debe estar entre 1 y %d\n", MAX_GRAPH_NODES);
        return;
    }
    
    graph_free(graph);
    *graph = graph_create((size_t)nodes);
    if (*graph) {
        printf("Grafo creado con %d nodos\n", nodes);
    }
}

/**
 Maneja el comando 'read'. Es el núcleo de la lógica del programa.
 EXPLICACIÓN DEL Flujo:
 1. Abre el archivo y llena el grafo.
 2. Verifica si el grafo está conectado (requisito previo).
 3. Ejecuta el algoritmo TSP para buscar la ruta óptima.
 */
static void handle_read(char *path, Graph *graph) {
    if (!graph) {
        puts("Debe crear el grafo primero con: start <cantidad>");
        return;
    }
    if (!path) {
        puts("Uso: read <ruta_archivo>");
        return;
    }
    
    printf("Agregando enlaces desde archivo %s....\n", path);
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("No se pudo abrir el archivo");
        return;
    }
    
    graph_clear(graph); 
    char from, to;
    int cost;
    while (fscanf(fp, " %c %c %d", &from, &to, &cost) == 3) {
        if (!graph_add_edge(graph, from, to, cost)) {
            fprintf(stderr, "Advertencia: No se pudo agregar arista %c-%c\n", from, to);
        }
    }
    fclose(fp);

    puts("Verificando que existe una ruta....");
    if (!graph_is_connected(graph)) {
        puts("No existe un camino que recorra todas las ciudades y regrese a la ciudad de origen.");
        return;
    }

    HamiltonianResult result = tsp_find_cycle(graph);
    
    if (!result.exists) {
        puts("No existe un camino que recorra todas las ciudades y regrese a la ciudad de origen.");
    } else {
        puts("Existe un camino que recorre todas las ciudades y regresa a la ciudad de origen.");
        printf("Ruta a seguir: ");
        for (size_t i = 0; i < result.count; ++i) {
            printf("%c ", (char)('A' + result.order[i]));
        }
        printf("\nCosto total minimo: %d\n", result.cost);
    }
}

/*Maneja el comando 'graph'. Muestra la matriz.*/
static void handle_graph(Graph *graph) {
    if (graph) {
        graph_print(graph);
    } else {
        puts("Debe crear el grafo primero con: start <cantidad>");
    }
}

/* Bucle infinito que espera comandos. Es un REPL (Read-Eval-Print Loop).*/
int main(void) {
    Graph *graph = NULL;
    char buffer[256];

    puts("Planificador de Vendedor Viajante (pvv). Escriba help para ayuda.");
    
    while (1) {
        printf("pvv> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;

        char *line = trim(buffer);
        if (*line == '\0') continue;

        char *command = strtok(line, " ");
        char *arg = strtok(NULL, " ");

        if (!command) continue;

        if (strcmp(command, "exit") == 0) {
            puts("Limpiando cache y saliendo ...");
            break;
        } else if (strcmp(command, "help") == 0) {
            print_help();
        } else if (strcmp(command, "clear") == 0) {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        } else if (strcmp(command, "start") == 0) {
            handle_start(arg, &graph);
        } else if (strcmp(command, "read") == 0) {
            handle_read(arg, graph);
        } else if (strcmp(command, "graph") == 0) {
            handle_graph(graph);
        } else {
            printf("Comando desconocido: %s\n", command);
            print_help();
        }
    }

    graph_free(&graph);
    return 0;
}