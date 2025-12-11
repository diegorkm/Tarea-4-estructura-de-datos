CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g -I./incs
SRC = src/main.c src/graph.c src/tsp.c
OBJ = $(SRC:.c=.o)
BIN = pvv

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	# Ajustado para borrar el binario y los objetos en src/
	rm -f src/*.o $(BIN)

.PHONY: all clean