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

//I got load and unload from a stackoverflow page NOT MY ORIGINAL WORK
//these are temporary until I can fix the rest of the main method then I will
//replace these. Not sure exactly how to rewrite these tho... seems pretty straight
//forward tbh
void load(struct aiocb *fill, off_t off, size_t blockSize) {
	fill->aio_fildes = fileno(stdin);
	fill->aio_nbytes = blockSize; //blockSize
	fill->aio_offset = off;
	fill->aio_buf = malloc(blockSize);
	fill->aio_reqprio = 0;
}

void unload(struct aiocb *store, off_t off, size_t blockSize) {
	store->aio_fildes = fileno(stdout);
	store->aio_nbytes = blockSize;
	store->aio_offset = off;
}


void matrix_add (struct aiocb* calculate, int size, int scalar, int blockSize) {

}

void main (int argc, char** argv) {
	int startTime = time(NULL);
	int endTime = 0;
	srand(time(0));
	int scalar = (rand() % (100 + 100 + 1)) - 100;
	int size = atoi(argv[1]);
	int blocks = atoi(argv[2]);
	int bufferSize = atoi(argv[1]) * 5;
	int blocksSquared = (blocks * blocks);
	//this is because we have offset of 4 per number and we have
	//new lines at the end of each row
	int totChar = (4 * size * size) + (size + 1);
	int blockSize = totChar / blocks;


	struct aiocb calculate;	//gets filled first and calculations done here
	struct aiocb fill;	//this is the one that reads blocks
	struct aiocb store; //once calculate is done this writes back

	int i = 0;

	for (i = 0; i < totChar; i = i + blockSize) {
		if (i == 0) {

			load(&calculate, 0, blockSize);
			aio_read(&calculate);

			while(aio_error(&calculate) == EINPROGRESS);

			aio_return(&calculate);

			//filling next one
			load(&fill, i + blockSize, blockSize);
			aio_read(&fill);
			while(aio_error(&fill) == EINPROGRESS);
			aio_return(&fill);
			//filled next one

			matrix_add(&calculate, size, scalar, blockSize);

			//copy calculated one to write one
			memcpy(&store, &calculate, sizeof(struct aiocb));
			unload(&store, i, blockSize); //i - blockSize

			aio_write(&store);
			while(aio_error(&store) == EINPROGRESS);
			aio_return(&store);
			//stored
			//copy next one to calculate one
			memcpy(&calculate, &fill, sizeof(struct aiocb));
		}
		else if (i < totChar) {

			//filling next one
			load(&fill, i + blockSize, blockSize);
			aio_read(&fill);
			while(aio_error(&fill) == EINPROGRESS);
			aio_return(&fill);
			//filled next one
			//calculating the first filled
			matrix_add(&calculate, size, scalar, blockSize);
			
			//copy calculated one to write one
			memcpy(&store, &calculate, sizeof(struct aiocb));
			unload(&store, i, blockSize); //i - blockSize

			aio_write(&store);
			while(aio_error(&store) == EINPROGRESS);
			aio_return(&store);
			//stored
			//copy next one to calculate one
			memcpy(&calculate, &fill, sizeof(struct aiocb));
		}

	}

	matrix_add(&calculate, size, scalar, blockSize);

	memcpy(&store, &calculate, sizeof(struct aiocb));
	unload(&store, i, blockSize);

	aio_write(&store);
	while(aio_error(&store) == EINPROGRESS);
	aio_return(&store);

	endTime = time(NULL);
	int opTime = endTime - startTime;
	fprintf(stderr, "time for operation: %d\n", opTime);
}