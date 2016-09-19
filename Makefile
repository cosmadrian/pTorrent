CC=gcc
CFLAGS=-c -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings -Wunreachable-code -Os -ftrapv -v -da -Q -g -O0 
LDFLAGS=
SRC=main.c input_handler/input_handler.c command/command.c error/error.c
OBJ=main.o input_handler.o command.o error.o 
EXE=ptorrent


all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC) 

build:
	make $(EXE)

clean:
	rm -fr $(OBJ) *.c.*

run_1: $(EXE)
	./$(EXE)
