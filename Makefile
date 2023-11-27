# Author: Param Desai - assignment 4
# Modified to fit project criteria

CC=gcc
CFLAGS=-Wall -Wextra -Werror -g
OBJ=./out/main.o ./out/ghost.o ./out/hunter.o ./out/house.o ./out/logger.o ./out/utils.o
EXE=final.exe

all: $(EXE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)

.PHONY: clean valgrind

clean:
	rm -f $(OBJ) $(EXE)