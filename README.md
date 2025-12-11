# Tarea-4-estructura-de-datos

## Proyecto: Planificador de Vendedor Viajante (Tarea 4)

Este repositorio contiene una implementación en C del programa `pvv` descrito en la Tarea 4 de Estructuras de Datos. El binario funciona como una consola interactiva que permite:

1. Crear un grafo con hasta 26 nodos etiquetados desde `A`.
2. Leer aristas y costos desde un archivo de texto.
3. Verificar si existe un ciclo Hamiltoniano que recorra todas las ciudades y vuelva al origen.
4. Calcular el costo mínimo cuando existe dicho ciclo.
5. Visualizar el grafo actual.

## Lo que falta:

- **Informe escrito**: no está incluido; debe elaborarse por separado explicando la técnica y decisiones.
- **Pruebas adicionales**: el programa se probó con grafos pequeños de ejemplo, pero conviene crear más casos (5‑9 nodos como en la pauta).
- **Compatibilidad Windows**: el código es estándar C y el `Makefile` puede usarse con MinGW/WSL, pero no se ha verificado en ese entorno.
- **Documentación extensa**: en este archivo se detallan los módulos y funciones, pero el informe debe complementar con diagramas/justificaciones.

## Estructura del repositorio

```
.
├── Makefile
├── README.md
├── samples
│   ├── ruta1.txt
│   └── ruta2.txt
└── src
    ├── graph.c
    ├── main.c
    ├── tsp.c
├── incs
│   ├── graph.h
│   └── tsp.h
```

- `src/main.c`: interfaz de línea de comandos y orquestación de comandos `pvv`.
- `src/graph.[ch]`: representación del grafo mediante matriz de adyacencia, operaciones básicas y verificación de conectividad.
- `src/tsp.[ch]`: búsqueda del ciclo Hamiltoniano mínimo usando backtracking con poda.
- `samples/*.txt`: ejemplos de archivos de entrada.

## Compilación y ejecución

### Linux / macOS

```sh
make          # compila con gcc/clang
./pvv         # ejecuta el CLI
make clean    # elimina binarios
```

### Windows (MinGW o WSL)

1. Instalar un compilador compatible con C11 (MinGW-w64 o usar WSL).
2. Abrir la terminal correspondiente.
3. Ejecutar los mismos comandos anteriores (`make`, `pvv.exe`).

## Uso del programa

```
pvv start 5          # crea grafo con 5 nodos (A-E)
pvv read ruta.txt    # lee rutas, verifica conectividad y TSP
pvv graph            # imprime el grafo
pvv help             # muestra ayuda
pvv clear            # limpia la pantalla
pvv exit             # libera memoria y termina
```

### Formato del archivo de rutas

Cada línea: `NodoOrigen NodoDestino Costo`. Ejemplo:

```
A B 15
B C 20
C A 10
```

El grafo se considera no dirigido, por lo que `A B 15` implica el mismo costo en ambos sentidos.

## Explicación de archivos y funciones

### `src/main.c`

- **`print_help`** (L9‑16): lista los comandos permitidos.
- **`trim`** (L18‑33): elimina espacios al inicio/fin de la entrada.
- **`ensure_graph_exists`** (L35‑41): muestra un mensaje si el grafo aún no se ha creado.
- **`load_edges_from_file`** (L43‑76): abre el archivo indicado, reinicia el grafo y agrega cada arista; valida formato por línea e informa errores.
- **`print_route`** (L78‑88): imprime el ciclo Hamiltoniano encontrado y su costo mínimo.
- **`main`** (L90‑178): bucle principal tipo REPL. Interpreta los comandos:
  - `start <n>` (L111‑128): crea o recrea el grafo con `n` nodos.
  - `read <archivo>` (L130‑153): carga aristas, verifica conectividad y calcula el ciclo más corto.
  - `graph` (L156‑160): invoca `graph_print`.
  - `help` (L163‑165) y `exit` (L168‑170): comandos auxiliares.
  - Comandos desconocidos muestran ayuda.

### `src/graph.h`

Define la estructura `Graph`, el máximo de nodos (`MAX_GRAPH_NODES`, 26) y las funciones públicas:

- `graph_create`, `graph_free`, `graph_clear`: ciclo de vida de la estructura.
- `graph_add_edge`: agrega aristas simétricas y valida etiquetas (`A-Z`).
- `graph_is_connected`: BFS para asegurar que todas las ciudades están conectadas (requisito previo).
- `graph_print`, `graph_weight`, `graph_index_from_label`: utilidades para mostrar y consultar el grafo.

### `src/graph.c`

Implementa las funciones anteriores:

- `allocate_matrix` (L7‑23): reserva memoria para la matriz de adyacencia.
- `graph_index_from_label` (L25‑33): convierte letras a índices.
- `graph_create` / `graph_clear` / `graph_free` (L35‑82): gestión del grafo y memoria.
- `graph_add_edge` (L95‑109): comprueba rangos y asigna costos simétricos.
- `graph_is_connected` (L111‑138): BFS usando una cola para asegurar conectividad.
- `graph_print` (L140‑155): muestra cada nodo seguido de sus vecinos y costos.

### `src/tsp.h` y `src/tsp.c`

- `HamiltonianResult`: estructura con la ruta encontrada (`order`), costo total y si existe.
- `tsp_find_cycle`: punto de entrada que inicia la búsqueda.
- `search_cycle` (L6‑52): función recursiva de backtracking:
  1. Si se visitaron todos los nodos, intenta cerrar el ciclo regresando a `A`.
  2. Actualiza el resultado si el costo total mejora.
  3. Explora nodos no visitados conectados al actual, haciendo poda si el costo acumulado ya supera el mejor encontrado.

## Sugerencias para el informe

- Explicar por qué se escogió backtracking (grafos de hasta 9 nodos permiten fuerza bruta).
- Describir el criterio de poda y cómo el BFS previo evita búsquedas innecesarias.
- Incluir diagramas o tablas que detallen los archivos y módulos de este repositorio.
