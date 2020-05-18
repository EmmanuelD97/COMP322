#Emmanoel Dermkrdichyan
#lab7 makefile

EXECUTABLEG=matrix_gen
SRCG=matrix_gen.c
OBJG=matrix_gen.o

EXECUTABLEA=matrix_add
SRCA=matrix_add.c
OBJA=matrix_add.o

CC=gcc
CFLAGS=-g

all: $(EXECUTABLEG) $(EXECUTABLEA)


$(EXECUTABLEG): $(SRCG)
	$(CC) $(CFLAGS) -c $(SRCG) -lrt
	$(CC) $(CFLAGS) -o $@ $(OBJG) -lrt

$(EXECUTABLEA): $(SRCA)
	$(CC) $(CFLAGS) -c $(SRCA) -lrt
	$(CC) $(CFLAGS) -o $@ $(OBJA) -lrt

CLEAN:
	rm -f $(EXECUTABLEG) $(OBJG) *~
	rm -f $(EXECUTABLEA) $(OBJA)
