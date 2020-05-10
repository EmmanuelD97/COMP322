/*
Emmanoel Dermkrdichyan - lab 6 - mole.c
4/30/20
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void mole(int argc, char **argv) {
	FILE *file = fopen("lab6.log", "a");
	char moleN[10];

	if (file != 0 && argc > 1) {
		sprintf(moleN, "Pop mole%d", atoi(argv[1]));
		fputs(moleN, file);

		fclose(file);
	}
}

void main (int argc, char **argv) {
	mole(argc, argv);
}