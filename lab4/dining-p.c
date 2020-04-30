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

/*
used to trap the term signal and set a flag to say
that it was received so we can exit successfully
*/
void signalHandler(int sigNumber) {
	signal (sigNumber, signalHandler);
	printf("SIGTERM received\n");
	termFlag = 1;
}


/*
Just printing which philosopher is eating and
then putting the process to sleep for a random time
*/
void eating (int pos) {
	printf("Philosopher #%d is eating\n", pos);
	//take up random time usleep(3) rand(3)
	usleep(rand() % 888888);
}

/*
Just printing which philosopher is thinking and
then putting the process to sleep for a random time
*/
void thinking (int pos) {
	printf("Philosopher #%d is thinking\n", pos);
	usleep(rand() % 888888);
}


/*
if the term flag is set to 1 we exit the do while loop in main and
get called into this function which destroys all the semaphores and
exits successfully once its done with that
*/
void termReceived(int position, int seats, int totalCycles, sem_t** semAddr, sem_t* semCrit) {
	char chop[10];

	//each process prints out its own statement with cycle count and position
	fprintf(stderr, "Philosopher #%d completed %d cycles.\n", position, totalCycles);

	if (position == seats){
		sem_close(semAddr[position]);
		sem_close(semAddr[0]);

		//if position is 0 it creates a string chopSt0 and then
		//uses that string to unlink that semaphore
		sprintf(chop, "chopSt%d", position); //unlinking the "left" side
		sem_unlink(chop);

		//this is to unlink the "right" side
		//0 for right because it links around to the other side
		sprintf(chop, "chopSt%d", 0);
		sem_unlink(chop);

		sem_destroy(semAddr[position]); //destroy "left"
		sem_destroy(semAddr[0]); //destroy "right"
	}
	else {
		//same as the above if statement except for every other case
		//that is not the last position in the table
		//so there is no need to loop around and get the first semaphore
		sem_close(semAddr[position]);
		sem_close(semAddr[position + 1]);

		sprintf(chop, "chopSt%d", position);
		sem_unlink(chop);

		sprintf(chop, "chopSt%d", position + 1);
		sem_unlink(chop);

		sem_destroy(semAddr[position]);
		sem_destroy(semAddr[position + 1]);
	}

	//this is destroying the semaphore used for the critical section
	sem_close(semCrit);
	sem_unlink("criticalSec");
	sem_destroy(semCrit);

	exit(EXIT_SUCCESS);
}

void groupIDSet() {
    FILE *file;
    struct stat sb;
    char* read = NULL;
    size_t length = 0;


    if (stat("pgid.txt", &sb) == 0) { //if file exists
        file = fopen("pgid.txt", "r"); //open for reading

        getline(&read, &length, file); //read the pgid

        fclose(file);
    } 
    else { //file doesn't exist
        char writer[64];
        memset(writer, '\0', 64);
        file = fopen("pgid.txt", "w"); 

        //writing the pgid into the file so we can use it to 
        //end all the processes later
        sprintf(writer, "%d", getpgid(getpid()));
        fwrite(writer, sizeof (char), sizeof (writer), file);

        fclose(file);
    }
}

int criticalSection(int ret, sem_t* criticalSem) {
    if (ret == 2) {
        if (sem_trywait(criticalSem) < 0) { //wait on the critical section semaphore
        	return 0;
        }
        else {
        	return 1;
        }
    } 
    else if (ret == 3) {
        sem_post(criticalSem); //free the critical section semaphore
        return 1;
    }
    else {
    	return 1;
    }
}


int main(int argc, char **argv) {
	sem_t** semAddr; //used for the chopsticks
	sem_t* semCrit; //used for the critical section
	groupIDSet(); //setting the pgid for all the processes
	int cycleCt = 0; //keeps track of each phylosophers cycle count
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

	//SIGTERM handler
	signal(SIGTERM, signalHandler);

	//openning the semaphores for the chopsticks
	for (int i = 0; i < seats; i++) {
		sprintf(str, "chopSt%d", i);
		semAddr[i] = sem_open(str, O_CREAT, 0666, 1);
	}

	//openning the semaphore for the critical section
	semCrit = sem_open("criticalSec", O_CREAT, 0666, 1); //used to moderate critical section

	//the cycle where phylosophers eat and think
	do {
		if(criticalSection(2, semCrit) == 1) { //make sure not in critical section
			//if the semaphores are free they eat
			if ((sem_wait(semAddr[position % seats])) != -1 && (sem_wait(semAddr[(position + 1) % seats]) != -1)) {
				eating(position);
			}
			//done eating now post the semaphore
			if ((sem_post(semAddr[position % seats])) != -1 && (sem_post(semAddr[(position + 1) % seats]) != -1)) {
				thinking(position);
			}
			//increment the count for the semaphores
			cycleCt++;
			//call to free the semaphore for the critical section
			criticalSection(3, semCrit);
		}

	} while (termFlag == 0);

	//terminate signal received so we destroy semaphores and
	//exit successfully
	termReceived(position, seats, cycleCt, semAddr, semCrit);

}