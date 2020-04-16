/*
Emmanoel Dermkrdichyan Lab 4 - Rice, Rice, and Rice
4/12/20
*/
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

void termHandler(int signum) {
	//remove the philosopher from the eat-think cycle
	//release any system resources sem_destroy(3) sem_close(3) sem_unlink(3)

	fprintf(stderr, "Philosopher #%d completed %d cycles\n", pNum, numCycles);
	exit(EXIT_SUCCESS);
}

void dining(int argc, char **argv) {
	sem_t chopstick[5];

	signal(SIGTERM, termHandler);


	do {
		wait(chopstick[i]);
		wait(chopstick[(i + 1) % 5]);


		signal(chopstick[i]);
		signal(chopstick[(i + 1) % 5]);

		//signal(SIGTERM, signal_handler);

	} while (true);
}

void eating (int pNum) {
	printf("Philosopher #%d is eating\n", pNum);
	//take up random time usleep(3) rand(3)
	usleep(rand());
}

void thinking (int pNum) {
	printf("Philosopher #%d is thinking\n", pNum);
	usleep(rand());
}

int main(int argc, char **argv) {
  /*
  Catcher takes care of main logic, it brings all the functions together.
  */
  dining(argc, argv);

  return 0;
}