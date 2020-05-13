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

static pid_t mole1;  //were using these in the signal handler so
static pid_t mole2;  //we need to have them as global

int termReceived = 0;

void signalHandler(int sigNumber) {
	int randMolNum = 0;
	char* moleNum;
	char* pickedName;

	if (sigNumber == SIGTERM) {
		termReceived = 1;
		kill(mole1, SIGKILL);
		kill(mole2, SIGKILL);

		exit(EXIT_SUCCESS);
	}
	else if (sigNumber == SIGUSR1 || sigNumber == SIGUSR2) {
		if (sigNumber == SIGUSR1 && mole1 != 0) {
			kill(mole1, SIGKILL);
		}
		else if (sigNumber == SIGUSR2 && mole2 != 0) {
			kill(mole2, SIGKILL);
		}

		signal(sigNumber, signalHandler);

		randMolNum = (rand() % (2 - 1 + 1)) + 1;

		char* moleV[] = {"mole", randMolNum, NULL};

		if (randMolNum == 1) {
			if (mole1 != 0) {
				kill(mole1, SIGKILL); //SIGCHLD //i dont think im killing the moles correctly
			}

			mole1 = fork();			// so its not limiting their number

			if (mole1 == 0) {
				execv(moleV[0], moleV); 
			}
		}
		else if (randMolNum == 2) {
			if (mole2 != 0) {
				kill(mole2, SIGKILL); //SIGCHLD SIGKILL
			}

			mole2 = fork();

			if (mole2 == 0) {
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
		session = setsid(); //making daemon session leader

		//registering the signals
		signal(SIGUSR1, signalHandler);
		signal(SIGUSR2, signalHandler);
		signal(SIGTERM, signalHandler);

		//opening the descriptor as a read only
		descriptor = open("/dev/null", O_RDONLY);
		//going to root directory
		chdir("/");

		//duplicating a file descriptor from descriptor to
		//the new ones like STDOUT STDERR STDIN
		dup2(descriptor, STDOUT_FILENO);
		dup2(descriptor, STDERR_FILENO);
		dup2(descriptor, STDIN_FILENO);
		close(descriptor);

		//Pausing until we receive a term signal
		while (termReceived == 0) {
			pause();
		}

		//term signal was received and moles were killed
		return 2;
	}
	else {
		//printing the daemon pid and returning 1 to kill
		//the parent
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
		exit(EXIT_SUCCESS); //TERM signal was received
	}
}