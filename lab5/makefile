#Emmanoel Dermkrdichyan
#lab5 makefile

EXECUTABLE=lab5
SRC=lab5.c
OBJ=lab5.o

CC=gcc
CFLAGS=-g

all: $(EXECUTABLE)


$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

CLEAN:
	rm -f $(EXECUTABLE) $(OBJ) *~
