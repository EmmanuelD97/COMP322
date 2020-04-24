/*
Emmanoel Dermkrdichyan Lab 4 - Rice, Rice, and Rice
4/12/20
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

static int termFlag = 0;

static unsigned long next = 1;

void signalHandler(int sigNumber) {
	signal (SIGTERM, signalHandler);
	printf("SIGTERM %d\n", sigNumber);
	termFlag = 1;
}

/*
got this from man rand(3)
RAND_MAX is assumed to be 32767
*/
int myrand(void) {
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % 32768);
}

void mysrand(unsigned seed) {
    next = seed;
}


void eating (int pos) {
	printf("Philosopher #%d is eating\n", pos);
	//take up random time usleep(3) rand(3)
	usleep(random());
}

void thinking (int pos) {
	printf("Philosopher #%d is thinking\n", pos);
	usleep(random());
}

int dining(int argc, char **argv, sem_t chopstick[], int seats, int position) {
	int cycleCt = 0;
	signal(SIGTERM, signalHandler);

	do {
		sem_wait(&chopstick[position]);						  //atoi converts from string to int
		sem_wait(&chopstick[(position + 1) % seats]); //argv[2] is number of seats
		eating(position);

		sem_post(&chopstick[position]);
		sem_post(&chopstick[(position + 1) % seats]);
		thinking(position);

		cycleCt++;

	} while (termFlag == 0);

	return cycleCt;
}
void termReceived(int position, int totalCycles, sem_t chopstick[], char* semN) {
	fprintf(stderr, "Philosopher #%d completed %d cycles.", position, totalCycles);

	sem_close(&chopstick[position]);
	sem_unlink(semN); //semN needs to get changed
	sem_destroy(&chopstick[position]);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	int totalCycles;
	int seats = atoi(argv[1]);
	int position = atoi(argv[2]);
	sem_t chopstick[(seats + 1)];


	if(argc != 3) {
		printf("Wrong number of arguments. (needs 2)\n");
		exit(EXIT_FAILURE);
	}

	//int shm_fd = shm_open(argv[2], O_CREAT | O_EXCL, 0666);




	totalCycles = dining(argc, argv, chopstick, seats, position);

	termReceived(position, totalCycles, chopstick, argv[2]);



	//exit(EXIT_SUCCESS);
}