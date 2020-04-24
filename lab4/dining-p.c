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

#define SEM_FILE1  "/leftCh"
#define SEM_FILE2  "/RightCh"

sem_t * returnLeft;
sem_t * returnRight;
//sem_t * chopstick[atoi(argv[1]) + 1];

static int termFlag = 0;
static unsigned long next = 1;



void signalHandler(int sigNumber) {
	signal (SIGTERM, signalHandler);
	printf("SIGTERM received\n");
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
	//usleep(random());
	usleep(300);
}

void thinking (int pos) {
	printf("Philosopher #%d is thinking\n", pos);
	//usleep(random());
	usleep(300);
}

int dining(int argc, char **argv, sem_t chopstick[], int seats, int position) {
	int cycleCt = 0;
	printf("its before the signal\n");

	signal(SIGTERM, signalHandler);
	printf("its before the do while loop\n");

	do {
		printf("its before semwait1\n");
		sem_wait(returnLeft);  //atoi converts from string to int
		printf("its before semwait2\n");
		sem_wait(returnRight); //argv[2] is number of seats
		eating(position);

		printf("its before sempost1\n");
		//sem_post(&chopstick[position]);
		printf("its before sempost2\n");
		//sem_post(&chopstick[(position + 1) % seats]);
		thinking(position);

		cycleCt++;

	} while (termFlag == 0);

	return cycleCt;
}
void termReceived(int position, int totalCycles, sem_t chopstick[], char* semN) {
	fprintf(stderr, "Philosopher #%d completed %d cycles.\n", position, totalCycles);

	sem_close(&chopstick[position]);
	sem_unlink(semN); //semN needs to get changed
	sem_destroy(&chopstick[position]);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	printf("PID: %d\n", getpid());
	printf("literally first thing in main\n");

	int totalCycles;
	int seats = atoi(argv[1]);
	int position = atoi(argv[2]);
	sem_t chopstick[(seats + 1)];

	printf("its before the semaphore open\n");

	returnLeft = sem_open(SEM_FILE1, O_CREAT|O_EXCL, 0666, 1);
	returnRight = sem_open(SEM_FILE2, O_CREAT|O_EXCL, 0666, 1);

	printf("its before the argc check\n");

	if(argc != 3) {
		printf("Incorrect number of arguments. (needs 2)\n");
		exit(EXIT_FAILURE);
	}
	else if (position > seats) {
		printf("All seats filled\n");
	}


	printf("its before the call to dining\n");

	totalCycles = dining(argc, argv, chopstick, seats, position);

	termReceived(position, totalCycles, chopstick, argv[2]);



	//exit(EXIT_SUCCESS);
}