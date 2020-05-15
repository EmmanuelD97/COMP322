#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int M = 3;
const int N = 3;

void matrix_add (int block[M][N], int size, int scalar) {
	for (int i = 0; i < size; i++) {//he set i = 1 but doesnt make sense
		for (int j = 0; j < size; j++) { //again j = 1
			block[i][j] = block[i][j] + scalar;
		}
	}
}

void main (int argc, char** argv) {
	int startTime = time(NULL);
	srand(time(0));
	int scalar = (rand() % (100 + 100 + 1)) - 100;
	int size = atoi(argv[1]);
	int blocks = atoi(argv[2]);
	int blockSize = size / blocks;
	int bufferSize = atoi(argv[1]) * 5;


	char buffer[bufferSize];
	for (int i = 0; i < size; i++) {
		fgets(buffer, bufferSize, stdin);
		printf("%s", buffer);
	}
	//fgets(buffer, 10000, stdin);

	//printf("should be first: %d\nshould be second: %d\n", size,blocks);
	//printf("num of args: %d\n", argc);
	//size * size * 4 4k
	//block * block * 12 offset
	//matrix_add ();
	//printf("%d\n%d\n", startTime,scalar);

}