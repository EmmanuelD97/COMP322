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
#include <string.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

static int termFlag = 0;

void signalHandler(int sigNumber) {
	signal (sigNumber, signalHandler);
	printf("SIGTERM received\n");
	termFlag = 1;
}

void eating (int pos) {
	printf("Philosopher #%d is eating\n", pos);
	//take up random time usleep(3) rand(3)
	usleep(rand() % 888888);
}

void thinking (int pos) {
	printf("Philosopher #%d is thinking\n", pos);
	usleep(rand() % 888888);
}

void termReceived(int position, int seats, int totalCycles, sem_t** semAddr) {
	char chop[10];

	fprintf(stderr, "Philosopher #%d completed %d cycles.\n", position, totalCycles);

	if (position == seats){
		sem_close(semAddr[position]);
		sem_close(semAddr[0]);

		sprintf(chop, "chopSt%d", position);
		sem_unlink(chop);

		sprintf(chop, "chopSt%d", 0);
		sem_unlink(chop);


		sem_destroy(semAddr[position]);
		sem_destroy(semAddr[0]);
	}
	else {
		sem_close(semAddr[position]);
		sem_close(semAddr[position + 1]);

		sprintf(chop, "chopSt%d", position);
		sem_unlink(chop);

		sprintf(chop, "chopSt%d", position + 1);
		sem_unlink(chop);


		sem_destroy(semAddr[position]);
		sem_destroy(semAddr[position + 1]);
	}
	exit(EXIT_SUCCESS);
}

void groupFile() { //DELETE OR CHANGE
    FILE *fptr;
    struct stat buf;
    char* line = NULL;
    size_t len = 0;
    if (stat("pgid.txt", &buf) == 0) {
        fptr = fopen("pgid.txt", "r");
        getline(&line, &len, fptr);
        //printf("%d", getpid());
        if (setpgid(getpid(), strtol(line, NULL, 10)) != 0) {
            perror("setpgid() error");
        }
        fclose(fptr);
    } else {
        char buffer[32];
        memset(buffer, '\0', 32);
        fptr = fopen("pgid.txt", "w");
        sprintf(buffer, "%d", getpgid(getpid()));
        fwrite(buffer, sizeof (char), sizeof (buffer), fptr);
        fclose(fptr);
    }
}

int main(int argc, char **argv) {
	printf("PID: %d\n", getpid());
	sem_t** semAddr; //used to be global
	groupFile();
	int cycleCt = 0;
	int seats = atoi(argv[1]);
	int position = atoi(argv[2]);
	char str[10];
	semAddr = malloc(sizeof (sem_t *) * seats);

	if(argc != 3) {
		printf("Incorrect number of arguments. (needs 2)\n");
		exit(EXIT_FAILURE);
	}
	else if (position > seats) {
		printf("All seats filled\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGTERM, signalHandler);

	for (int i = 0; i < seats; i++) { //original way to open sem
		sprintf(str, "chopSt%d", i);
		semAddr[i] = sem_open(str, O_CREAT|O_EXCL, 0666, 1);
	}

	do {
		printf("POS: %d PID: %d\n", position, getpid());
		int ret1 = sem_wait(semAddr[position % seats]);
		int ret2 = sem_wait(semAddr[(position + 1) % seats]);
		printf("pos: %d %d %d\n", position,ret1,ret2);

		eating(position);

		int ret3 = sem_post(semAddr[position % seats]);
		int ret4 = sem_post(semAddr[(position + 1) % seats]);
		printf("pos: %d %d %d\n", position,ret3,ret4);

		thinking(position);
		cycleCt++;

	} while (termFlag == 0);


	/*for (int i = 0; i < seats; i++) {
	sem_close(semAddr[i]);

	sprintf(toper, "chopSt%d", i);
	sem_unlink(toper);

	sem_destroy(semAddr[i]);

	printf("trying to delete\n");

	}*/

	termReceived(position, seats, cycleCt, semAddr);


	//exit(EXIT_SUCCESS);
}