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

void termReceived(int position, int seats, int totalCycles, sem_t** semAddr, sem_t* semCrit) {
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

	sem_close(semCrit);
	sem_unlink("criticalSec");
	sem_destroy(semCrit);

	exit(EXIT_SUCCESS);
}

void groupIDSet() {
    FILE *file;
    struct stat sb;
    char* line = NULL;
    size_t len = 0;


    if (stat("pgid.txt", &sb) == 0) {
        file = fopen("pgid.txt", "r");

        getline(&line, &len, file);

        fclose(file);
    } else {
        char writer[64];
        memset(writer, '\0', 64);
        file = fopen("pgid.txt", "w");

        sprintf(writer, "%d", getpgid(getpid()));
        fwrite(writer, sizeof (char), sizeof (writer), file);

        fclose(file);
    }
}

int criticalSection(int returnVal, sem_t* criticalSem) {
    if (returnVal == 2) {
        if (sem_trywait(criticalSem) < 0) {
        	return 0;
        }
        else {
        	return 1;
        }
    } 
    else if (returnVal == 3) {
        sem_post(criticalSem);
        return 1;
    }
    else {
    	return 1;
    }
}


int main(int argc, char **argv) {
	//printf("PID: %d\n", getpid());
	sem_t** semAddr;
	sem_t* semCrit;
	groupIDSet();
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

	for (int i = 0; i < seats; i++) {
		sprintf(str, "chopSt%d", i);
		semAddr[i] = sem_open(str, O_CREAT, 0666, 1);
	}

	semCrit = sem_open("criticalSec", O_CREAT, 0666, 1); //used to moderate critical section

	do {
		if(criticalSection(2, semCrit) == 1) {
			if ((sem_wait(semAddr[position % seats])) != -1 && (sem_wait(semAddr[(position + 1) % seats]) != -1)) {
				eating(position);
			}
			if ((sem_post(semAddr[position % seats])) != -1 && (sem_post(semAddr[(position + 1) % seats]) != -1)) {
				thinking(position);
			}
			cycleCt++;
			criticalSection(3, semCrit);
		}

	} while (termFlag == 0);

	termReceived(position, seats, cycleCt, semAddr, semCrit);

}