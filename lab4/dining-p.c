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

sem_t** semAddr;

static int termFlag = 0;
static unsigned long next = 1;



void signalHandler(int sigNumber) {
	signal (SIGTERM, signalHandler);
	printf("SIGTERM received\n");
	termFlag = 1;
}

void eating (int pos) {
	printf("Philosopher #%d is eating\n", pos);
	//take up random time usleep(3) rand(3)
	usleep(rand() % 888888);
	//usleep(300);
}

void thinking (int pos) {
	printf("Philosopher #%d is thinking\n", pos);
	usleep(rand() % 888888);
	//usleep(300);
}

void termReceived(int position, int seats, int totalCycles, sem_t** semAddr) {
	char chop[10];

	fprintf(stderr, "Philosopher #%d completed %d cycles.\n", position, totalCycles);
	if (position == 0) {
		sem_close(semAddr[seats - 1]);
		sem_close(semAddr[position + 1]);

		sprintf(chop, "chopSt%d", seats - 1);
		sem_unlink(chop);

		sprintf(chop, "chopSt%d", position + 1);
		sem_unlink(chop);

		sem_destroy(semAddr[seats - 1]);
		sem_destroy(semAddr[position + 1]);
	}
	else if (position == seats){
		sem_close(semAddr[0]);
		sem_close(semAddr[position - 1]);

		sprintf(chop, "chopSt%d", position - 1);
		sem_unlink(chop);

		sprintf(chop, "chopSt%d", 0);
		sem_unlink(chop);


		sem_destroy(semAddr[position - 1]);
		sem_destroy(semAddr[0]);
	}
	else {
		sem_close(semAddr[position - 1]);
		sem_close(semAddr[position + 1]);

		sprintf(chop, "chopSt%d", position - 1);
		sem_unlink(chop);

		sprintf(chop, "chopSt%d", position + 1);
		sem_unlink(chop);


		sem_destroy(semAddr[position - 1]);
		sem_destroy(semAddr[position + 1]);
	}
	exit(EXIT_SUCCESS);
}

void groupFile() { //DELETE OR CHANGE
    FILE *fptr;
    struct stat buf;
    char* line = NULL;
    size_t len = 0;
    if (stat("groupID.txt", &buf) == 0) {
        fptr = fopen("groupID.txt", "r");
        getline(&line, &len, fptr);
        //printf("%d", getpid());
        if (setpgid(getpid(), strtol(line, NULL, 10)) != 0) {
            perror("setpgid() error");
        }
        fclose(fptr);
    } else {
        char buffer[32];
        memset(buffer, '\0', 32);
        fptr = fopen("groupID.txt", "w");
        sprintf(buffer, "%d", getpgid(getpid()));
        fwrite(buffer, sizeof (char), sizeof (buffer), fptr);
        fclose(fptr);
    }
}

int main(int argc, char **argv) {
	printf("PID: %d\n", getpid());
	//sem_t** semAddr; //used to be global
	groupFile();
	int cycleCt = 0;
	int seats = atoi(argv[1]);
	int position = atoi(argv[2]);
	char str[10];
	semAddr = malloc(sizeof (sem_t *) * seats);

	//sem_t chopstick[(seats + 1)];

	//returnLeft = sem_open(SEM_FILE1, O_CREAT|O_EXCL, 0666, 1);
	//returnRight = sem_open(SEM_FILE2, O_CREAT|O_EXCL, 0666, 1);

	char toper[10];
	for (int i = 0; i < 25; i++) {
		sem_close(semAddr[i]);

		sprintf(toper, "chopSt%d", i);
		sem_unlink(toper);

		sem_destroy(semAddr[i]);

		//printf("trying to delete\n");
	}

	if(argc != 3) {
		printf("Incorrect number of arguments. (needs 2)\n");
		exit(EXIT_FAILURE);
	}
	else if (position > seats) {
		printf("All seats filled\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGTERM, signalHandler);

	for (int i = 0; i < seats; i++) {
		sprintf(str, "chopSt%d", i);
		semAddr[i] = sem_open(str, O_CREAT|O_EXCL, 0666, 1);

		//printf("%s\n",str);
	}

	do {
		/*printf("PID: %d\n", getpid());
		sem_wait(semAddr[position - 1]);
		sem_wait(semAddr[position + 1]);
		eating(position);

		sem_post(semAddr[position - 1]);
		sem_post(semAddr[position + 1]);
		thinking(position);*/


		if (position == 0) {
			//printf("PID: %d\n", getpid());
			sem_wait(semAddr[seats - 1]);
			sem_wait(semAddr[position + 1]);
			eating(position);

			sem_post(semAddr[seats - 1]);
			sem_post(semAddr[position + 1]);
			thinking(position);
		}
		else if (position == seats){
			//printf("PID: %d\n", getpid());
			sem_wait(semAddr[position - 1]);
			sem_wait(semAddr[0]);
			eating(position);

			sem_post(semAddr[position - 1]);
			sem_post(semAddr[0]);
			thinking(position);
		}
		else {
			//printf("PID: %d\n", getpid());
			sem_wait(semAddr[position - 1]);
			sem_wait(semAddr[position + 1]);
			eating(position);

			sem_post(semAddr[position - 1]);
			sem_post(semAddr[position + 1]);
			thinking(position);
		}

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