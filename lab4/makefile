#Emmanoel Dermkrdichyan
#lab4 makefile

EXECUTABLE=dining-p
SRC=dining-p.c
OBJ=dining-p.o

CC=gcc
CFLAGS=-g

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -pthread -o $@ $(OBJ)

CLEAN:
	rm -f $(EXECUTABLE) $(OBJ) *~
	rm -f pgid.txt
