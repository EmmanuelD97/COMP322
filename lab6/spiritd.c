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
		//kill both moles and set termflag to 1
		//to indicate exiting successfully
		termReceived = 1;
		kill(mole1, SIGKILL);
		kill(mole2, SIGKILL);
	}
	else if (sigNumber == SIGUSR1 || sigNumber == SIGUSR2) {
		if (sigNumber == SIGUSR1 && mole1 != 0) {
			//if SIGUSR1 and mole1 is not the child kill mole1
			kill(mole1, SIGKILL);
		}
		else if (sigNumber == SIGUSR2 && mole2 != 0) {
			//if SIGUSR2 and mole2 is not the child kill mole2
			kill(mole2, SIGKILL);
		}

		signal(sigNumber, signalHandler);

		//picks a random number either 1 or 2
		randMolNum = (rand() % (2 - 1 + 1)) + 1;

		char* moleV[] = {"mole", moleNum, NULL};

		//if picks number1
		//kill mole1 and create a new mole1
		if (randMolNum == 1) {
			if (mole1 != 0) {
				kill(mole1, SIGKILL);
			}

			mole1 = fork(); //new mole1

			if (mole1 == 0) {
				execv(moleV[0], moleV); 
			}
		}
		//if picks number2
		//kill mole2 and create a new mole2
		else if (randMolNum == 2) {
			if (mole2 != 0) {
				kill(mole2, SIGKILL);
			}

			mole2 = fork(); //new mole2

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

	//calling the process that forks for daemon
	//and starts the entire process
	procRet = daemonFork();

	if (procRet == 1) {
		exit(EXIT_SUCCESS); //killing parent
	}
	else if (procRet == 2) {
		exit(EXIT_SUCCESS); //TERM signal was received
	}
}