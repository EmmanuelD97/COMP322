#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <aio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

const int M = 3;
const int N = 3;


void load(struct aiocb *temp, off_t off, size_t blockSize) {
	memset(temp, 0, sizeof (struct aiocb));
	temp->aio_fildes = fileno(stdin);
	temp->aio_nbytes = blockSize; //blockSize
	temp->aio_offset = off;
	temp->aio_buf = malloc(blockSize);
	temp->aio_reqprio = 0;
}

void unload(struct aiocb *temp, off_t off, size_t blockSize) {
	temp->aio_fildes = fileno(stdout);
	temp->aio_nbytes = blockSize;
	temp->aio_offset = off;
}


void matrix_add (int block[M][N], int size, int scalar) {
	for (int i = 0; i < size; i++) {//he set i = 1 but doesnt make sense
		for (int j = 0; j < size; j++) { //again j = 1
			block[i][j] = block[i][j] + scalar;
		}
	}
}

void main (int argc, char** argv) {
	int startTime = time(NULL);
	int endTime = 0;
	srand(time(0));
	int scalar = (rand() % (100 + 100 + 1)) - 100;
	int size = atoi(argv[1]);
	int blocks = atoi(argv[2]);
	int bufferSize = atoi(argv[1]) * 5;
	//int last, next;
	int blocksSquared = (blocks * blocks);
	int blockSize = size / blocks;
	//this is because we have offset of 4 per number and we have
	//new lines at the end of each row
	int totChar = (4 * size * size) + size;


	struct aiocb calculate;	//gets filled first and calculations done here
	struct aiocb fill;	//this is the one that reads blocks
	struct aiocb store; //once calculate is done this writes back

    load(&calculate, 0, blockSize);
	aio_read(&calculate);

	while(aio_error(&calculate) == EINPROGRESS);

	aio_return(&calculate);
	//calculateed the first block now for the for loop

	for (int i = blockSize; i < totChar; i = i + blockSize) {
		//this is where we load the next set before calculating for
		//the first loaded block
		load(&fill, i, blockSize);
		aio_read(&fill);

		while(aio_error(&calculate) == EINPROGRESS);
		aio_return(&fill);
		//finished filling next block

	}

	//matrix_add();


	//memset(check, 0xaa, BUF_SIZE);
	//memset(&aiocb, 0, sizeof(struct aiocb));
	/*aiocb.aio_fildes = descriptor;
	aiocb.aio_buf = check;
	aiocb.aio_nbytes = BUF_SIZE;*/
	//aiocb.aio_lio_opcode = write;


	//int ret;
	//aio_read(&aiocb);
	//wait until end of transaction
	//while ((err = aio_error(&aiocb)) == EINPROGRESS);

	//err = aio_error(&aiocb);
	//ret = aio_return(&aiocb);

	//aio_read(&aiocb);

	//aio_return(&aiocb);


	for (int i = 0; i < blocksSquared - 1; i++) {
		//last = current - 1;
		//next = current + 1;

		//memcpy(next, calculate, 5 * blocksSquared);

		//calculate->aio_offset = 5 * blocksSquared * next;

		//aio_read(&calculate);

	}



	//fgets(buffer, 10000, stdin);

	//printf("should be first: %d\nshould be second: %d\n", size,blocks);
	//printf("num of args: %d\n", argc);
	//size * size * 4 4k
	//block * block * 12 offset
	sleep(3);
	//matrix_add ();
	//printf("%d\n%d\n", startTime,scalar);
	endTime = time(NULL);
	int opTime = endTime - startTime;
	printf("time for operation: %d", opTime);
}