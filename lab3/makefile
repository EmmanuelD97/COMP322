#Emmanoel Dermkrdichyan
#lab3 makefile

EXECUTABLE=catcher
SRC=catcher.c
OBJ=catcher.o

CC=gcc
CFLAGS=-g

all: $(EXECUTABLE)


$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

CLEAN:
	rm -f $(EXECUTABLE) $(OBJ) *~
