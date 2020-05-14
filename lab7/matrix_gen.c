#include <stdlib.h>
#include <stdio.h>

void matrixGen(int size) {
	int randomElement = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++){
			randomElement = (rand() % (100 + 100 + 1)) - 100;
		}
	}
}

void main (int argc, char** argv) {
	int size = 0;
	size = atoi(argv[1]);

	matrixGen(size);
}