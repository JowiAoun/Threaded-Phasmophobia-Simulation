# Author: Param Desai - assignment 4
# Modified to fit project criteria

CC=gcc
CFLAGS=-Wall -Wextra -Werror -g
OBJ=main.o ghost.o hunter.o house.o logger.o utils.o
EXE=final

all: $(EXE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)

.PHONY: clean valgrind

clean:
	rm -f $(OBJ) $(EXE)