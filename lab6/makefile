#Emmanoel Dermkrdichyan
#lab6 makefile

EXECUTABLE=spiritd
SRC=spiritd.c
OBJ=spiritd.o

EXECUTABLEM=moles
SRCM=moles.c
OBJM=moles.o

CC=gcc
CFLAGS=-g

all: $(EXECUTABLE) $(EXECUTABLEM)


$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(EXECUTABLEM): $(SRCM)
	$(CC) $(CFLAGS) -c $(SRCM)
	$(CC) $(CFLAGS) -o $@ $(OBJM)

CLEAN:
	rm -f $(EXECUTABLE) $(OBJ) *~
	rm -f $(EXECUTABLEM) $(OBJM)
