/*
Emmanoel Dermkrdichyan lab3 catcher
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>
#include <time.h>

int terminator = 0; //term signal counter
int sigsCaught = 0; //number of signals caught by handler

static const char signalInp[31][6] = {"HUP","INT","QUIT","ILL","TRAP","ABRT","EMT","FPE"
                                      ,"KILL","BUS","SEGV","SYS","PIPE","ALRM","TERM","URG"
                                      ,"STOP","STP","CONT","CHILD","TTIN","TTOU","IO","XCPU"
                                      ,"XFSZ","VTALRM","PROF","WINCH","INFO","USR1","USR2"};

void printPID() {
  int pid = getpid();
  fprintf(stderr, "catcher: $$ = %d\n", pid);
}

void printTotalCT(int totalSigCount) {
  fprintf(stderr, "catcher: Total signals count = %d\n", totalSigCount);
}

int whatToCatch(int argc, char **argv, int numberOfSigArgs, int indexToCatch[]) {
  int count = 0;
  for (int i = 1; i <= numberOfSigArgs; i++) {
    for (int j = 0; j < 31; j++) {
      if (strcmp(signalInp[j], argv[i]) == 0) {
        indexToCatch[count] = j;
        count++;
      }
    }
  }
}

void handler(int sig) {

  printf("SIG%s caught at %ld\n", signalInp[sig - 1],time(NULL));//change 1 to the index of signal caught
  sigsCaught++;

  if(sig == 15) {
    terminator++;
  }
  else if(sig != 15) {
    terminator = 0; //resetting the term counter until we have 3 consequtive terms
  }
}

int hasNonSig(int argc, char **argv) {
  if (argv[argc - 1][0] == '>' || argv[argc - 1][0] == '|') {
    return argc - 1;
  }
  else {
    return argc;
  }
}

void catcher(int argc, char **argv, int indexToCatch[]) {
  int nonSig = 0;
  int argCount = argc;

  printPID();

  nonSig = hasNonSig(argc, argv);
  argCount = argCount - nonSig;

  for (int i = 1; i < argc; i++) { //argc used to be argCount
    for (int j = 0; j < 31; j++) {
      if (strcmp(argv[i],signalInp[j]) == 0) {
        signal(j + 1, handler);
        if (signal(j + 1, handler) == SIG_ERR) {
          fprintf(stderr, "signal error\n");
        }
      }
    }
  }
  while (terminator < 3) {
    pause();
  }
  printTotalCT(sigsCaught);
}

int main(int argc, char **argv) {
  //im initializing these here so I can pass them between handler and catcher
  //without resetting the values of both
  int numberOfSigArgs = (hasNonSig(argc, argv) - 1);
  int indexToCatch[numberOfSigArgs];

  whatToCatch(argc, argv, numberOfSigArgs, indexToCatch);

  catcher(argc, argv, indexToCatch);
  return 0;
}
