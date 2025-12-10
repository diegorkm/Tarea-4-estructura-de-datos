CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
SRC := src/main.c src/graph.c src/tsp.c
OBJ := $(SRC:.c=.o)
BIN := pvv

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
