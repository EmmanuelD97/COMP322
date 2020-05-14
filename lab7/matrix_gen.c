/*
Emmanoel Dermkrdichyan - lab 7 - matrix_gen
5/14/20
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
Since were just using > to place the output into a file all
we have to do is print the random numbers that we are generating
in the shape of the matrix.
*/
void matrixGen(int size) {
	int randomElement = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++){
			//generates a random integer from -100 to 100
			randomElement = (rand() % (100 + 100 + 1)) - 100;
			printf("%d ", randomElement);
		}
		printf("\n");
	}
}

void main (int argc, char** argv) {
	//seed for random
	srand(time(0));
	int size = 0;
	size = atoi(argv[1]); //first argument is the size of the matrix

	matrixGen(size);
}