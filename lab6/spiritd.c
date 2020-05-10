/*
Emmanoel Dermkrdichyan - lab 6 - spiritd.c
4/30/20
*/

#include <stdio.h> //sprintf()
#include <stdlib.h> //exit()
#include <fcntl.h>	//open()
#include <unistd.h> //execv(),setsid(),chdir(), etc
#include <signal.h>	//signal()
#include <sys/stat.h>	//umask()

static pid_t firstMole;  //were using these in the signal handler so
static pid_t secondMole;  //we need to have them as global

int termReceived = 0;

void killMoles() {


}

void signalHandler(int sigNumber) {
	int randMolNum = 0;
	char* moleNum;
	char* pickedName;

	if (sigNumber == SIGTERM) {
		termReceived = 1;
		killMoles();
	}
	else {
		signal(sigNumber, signalHandler);
		randMolNum = (rand() % (2 - 1 + 1)) + 1;

		sprintf(moleNum, "%d", randMolNum);
		char* moleV[] = {"mole", moleNum, NULL};

		sprintf(pickedName, "mole%d", randMolNum);

		if (randMolNum == 1) {
			kill(firstMole, SIGCHLD);
			firstMole = fork();

			if (firstMole == 0) {
				execv(moleV[0], moleV);
			}
		}
		else if (randMolNum == 2) {
			kill(firstMole, SIGCHLD);
			secondMole = fork();

			if (secondMole == 0) {
				execv(moleV[0], moleV);
			}
		}

	}
}


void main (int argc, char** argv) {
	int procRet = 0;

}