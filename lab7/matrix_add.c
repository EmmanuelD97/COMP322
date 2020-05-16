#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <aio.h>

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

	#define BUF_SIZE 111
	unsigned char buf[BUF_SIZE];
	unsigned char check[BUF_SIZE];

	struct aiocb aiocb;

	memset(check, 0xaa, BUF_SIZE);
	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes = descriptor;
	aiocb.aio_buf = check;
	aiocb.aio_nbytes = BUF_SIZE;
	aiocb.aio_lio_opcode = write;

	if (aio_read(&aiocb) == -1) {
		printf("aio_read error\n");
		exit(EXIT_FAILURE);
	}

	//int err;
	//int ret;
	aio_read(&aiocb);
	//wait until end of transaction
	while ((err = aio_error(&aiocb)) == EINPROGRESS);

	//err = aio_error(&aiocb);
	//ret = aio_return(&aiocb);

	aio_read(&aiocb);

	aio_return(&aiocb);

	char buffer[bufferSize];
	for (int i = 0; i < size; i++) {
		fgets(buffer, bufferSize, stdin);
		printf("%s", buffer);
	}

	for (int x = 1, y = 1; x < blocks && y < blocks; x++, y++) {

	}


	/*


		last = current - 1;
		next - current + 1;

		async read request next      see aio_read(2)    

		matrix_add(current, block_size, scalar)

		async write request last      see aio_write(2)   
		async write return last          see aio_return(2) 
		     memcpy current → last       see memcpy(3)    

		async read return next         see aio_return(2) 
		     memcpy next → current



	*/



	//fgets(buffer, 10000, stdin);

	//printf("should be first: %d\nshould be second: %d\n", size,blocks);
	//printf("num of args: %d\n", argc);
	//size * size * 4 4k
	//block * block * 12 offset
	//matrix_add ();
	//printf("%d\n%d\n", startTime,scalar);

}