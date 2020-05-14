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
	$(CC) $(CFLAGS) -c $(SRCG)
	$(CC) $(CFLAGS) -o $@ $(OBJG)

$(EXECUTABLEA): $(SRCM)
	$(CC) $(CFLAGS) -c $(SRCA)
	$(CC) $(CFLAGS) -o $@ $(OBJA)

CLEAN:
	rm -f $(EXECUTABLEG) $(OBJG) *~
	rm -f $(EXECUTABLEA) $(OBJA)
