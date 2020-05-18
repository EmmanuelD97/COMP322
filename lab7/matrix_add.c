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
	//int last, next;
	int blocksSquared = (blocks * blocks);
	int blockSize = size / blocks;
	//this is because we have offset of 4 per number and we have
	//new lines at the end of each row
	int totChar = (4 * size * size) + size;


	struct aiocb calculate;	//gets filled first and calculations done here
	struct aiocb fill;	//this is the one that reads blocks
	//struct aiocb store; //once calculate is done this writes back

    load(&calculate, 0, blockSize);
	aio_read(&calculate);

	while(aio_error(&calculate) == EINPROGRESS);

	aio_return(&calculate);
	//add the matrix the first time
	matrix_add(&calculate, size, scalar, blockSize);

	unload(&calculate, 0, blockSize); //added first one now unload it struct aiocb *temp, off_t off, size_t blockSize

	aio_write(&calculate);

	for (int i = blockSize; i < totChar; i = i + blockSize) {
		//this is where we load the next set before calculating for
		//the first loaded block
		load(&fill, i, blockSize);
		aio_read(&fill);

		while(aio_error(&fill) == EINPROGRESS);
		aio_return(&fill);

		memcpy(&calculate, &fill, sizeof(&fill));

		matrix_add(&calculate, size, scalar, blockSize);

		unload(&calculate, (i - blockSize), blockSize);

		aio_write(&calculate);



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



	//fgets(buffer, 10000, stdin);

	//printf("should be first: %d\nshould be second: %d\n", size,blocks);
	//printf("num of args: %d\n", argc);
	//size * size * 4 4k
	//block * block * 12 offset
	//sleep(3);
	//matrix_add ();
	//printf("%d\n%d\n", startTime,scalar);
	endTime = time(NULL);
	int opTime = endTime - startTime;
	printf("time for operation: %d\n\n", opTime);
}