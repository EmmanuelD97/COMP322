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

/*struct aiocb { //I GOT THIS FROM THE MAN PAGES
                The order of these fields is implementation-dependent 

               int             aio_fildes;      File descriptor 
               off_t           aio_offset;      File offset 
               volatile void  *aio_buf;         Location of buffer 
               size_t          aio_nbytes;      Length of transfer 
               int             aio_reqprio;     Request priority 
               struct sigevent aio_sigevent;    Notification method 
               int             aio_lio_opcode;  Operation to be performed;
                                                  lio_listio() only 

                Various implementation-internal fields not shown 
};*/

void fill(struct aiocb *temp, off_t off, size_t blockSize) {
	memset(temp, 0, sizeof (struct aiocb));
	temp->aio_fildes = fileno(stdin);
	temp->aio_nbytes = blockSize; //blockSize
	temp->aio_offset = off;
	temp->aio_buf = malloc(blockSize);
	temp->aio_reqprio = 0;
}

void empty(struct aiocb *temp, off_t off) {
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
	int blockSize = size / blocks;
	int bufferSize = atoi(argv[1]) * 5;
	int last, next;
	int blocksSquared = (blocks * blocks);

	struct aiocb fill;
	struct aiocb next;

    fill(&fill, 0);
	aio_read(&fill);

	while(aio_error(&fill) == EINPROGRESS);

	aio_return(&fill);

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

	/*char buffer[bufferSize];
	for (int i = 0; i < size; i++) {
		fgets(buffer, bufferSize, stdin);
		printf("%s", buffer);
	}*/

	for (int i = 0; i < blocksSquared - 1; i++) {
		last = current - 1;
		next = current + 1;

		memcpy(next, fill, 5 * blocksSquared);

		fill->aio_offset = 5 * blocksSquared * next;

		aio_read(&fill);
		
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
	sleep(3);
	//matrix_add ();
	//printf("%d\n%d\n", startTime,scalar);
	endTime = time(NULL);
	int opTime = endTime - startTime;
	printf("time for operation: %d", opTime);
}