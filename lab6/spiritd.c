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
#include <sys/types.h> //wait
#include <sys/wait.h> //wait
#include <time.h> //i think rand()?

static pid_t firstMole;  //were using these in the signal handler so
static pid_t secondMole;  //we need to have them as global

int termReceived = 0;

int moleBackUp1;
int moleBackUp2;

void signalHandler(int sigNumber) {
	int randMolNum = 0;
	char* moleNum;
	char* pickedName;

	if (sigNumber == SIGTERM) {
		termReceived = 1;
		kill(firstMole, SIGKILL);
		kill(secondMole, SIGKILL);

		exit(EXIT_SUCCESS);
	}
	else if (sigNumber == SIGUSR1 || sigNumber == SIGUSR2) {
		if (sigNumber == SIGUSR1 && firstMole != 0) {
			kill(firstMole, SIGKILL); //SIGCHLD
		}
		else if (sigNumber == SIGUSR1 && secondMole != 0) {
			kill(secondMole, SIGKILL); //SIGCHLD
		}
		/* 
		Upon SIG_USR1, the program will
		kill child process #1 (mole1)
		randomly create either mole1 or mole 2 if it does not already exists
		Upon SIG_USER2, the program will
		kill child process #1 (mole2)
		randomly create either mole1 or mole 2 if it does not already exists

		*/
		signal(sigNumber, signalHandler);

		randMolNum = (rand() % (2 - 1 + 1)) + 1;

		char* moleV[] = {"mole", moleNum, NULL};

		if (randMolNum == 1) {
			if (firstMole == moleBackUp1) {
				kill(firstMole, SIGKILL); //SIGCHLD //i dont think im killing the moles correctly
			}
			firstMole = fork();			// so its not limiting their number
			moleBackUp1 = firstMole;

			if (firstMole == 0) {
				execv(moleV[0], moleV); 
			}
		}
		else if (randMolNum == 2) {
			if (secondMole == moleBackUp2) {
				kill(firstMole, SIGKILL); //SIGCHLD SIGKILL
			}
			secondMole = fork();
			moleBackUp2 = secondMole;

			if (secondMole == 0) {
				execv(moleV[0], moleV);
			}
		}

	}
}

int daemonFork() {
	int descriptor;
	int chdirRetVal;
	pid_t daemon;
	pid_t session;

	daemon = fork();

	if (daemon == 0) {
		umask(0); //file creation mask to 0
		session = setsid();

		signal(SIGUSR1, signalHandler);
		signal(SIGUSR2, signalHandler);
		signal(SIGTERM, signalHandler);

		descriptor = open("/dev/null", O_RDONLY);
		chdir("/");

		dup2(descriptor, STDOUT_FILENO);
		dup2(descriptor, STDERR_FILENO);
		dup2(descriptor, STDIN_FILENO);
		close(descriptor);
		while (termReceived == 0) {
			pause();
		}

		return 2;
	}
	else {
		printf("daemon: %d\n", daemon);
		return 1;
	}

}


void main (int argc, char** argv) {
	int procRet = 0;

	procRet = daemonFork();

	if (procRet == 1) {
		exit(EXIT_SUCCESS); //killing parent
	}
	else if (procRet == 2) {
		exit(EXIT_SUCCESS);
	}
}