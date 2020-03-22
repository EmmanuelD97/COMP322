/*
Emmanoel Dermkrdichyan lab3 catcher
*/

#include <stdio.h>
#include <sys/types.h>
#include<signal.h>
#include <unistd.h>
#include <sys/errno.h>

void printPID() {
  int pid = getpid();
  fprintf(stderr, "catcher: $$ = %d\n", pid);
}

void printTotalCT(int totalSigCount) {
  fprintf(stderr, "catcher: Total signals count = %d\n", totalSigCount);
}

void handler(int sig) {
  /*if (sig == SIGINT) {
  }*/

}

int hasNonSig(int argc, char **argv) {
  if (argv[argc - 1][0] == '>' || argv[argc - 1][0] == '|') {
    return 1;
  }
  else {
    return 0;
  }
}

void catcher(int argc, char **argv, int termCount, int totalSigCount) {
  int nonSig = 0;
  int argCount = argc;
  int *toSend;
  printPID();


  nonSig = hasNonSig(argc, argv);
  if (nonSig == 1) {
    argCount--;
  }

  for (int i = 1; i < argCount; i++) {
    //toSend = argv[i];
    signal(*argv[i], handler);
    totalSigCount++;
  }
  if (signal(SIGINT, handler) == SIG_ERR) {
    fprintf(stderr, "signal error\n");
  }


  //pause();

  printTotalCT(totalSigCount);
}

int main(int argc, char **argv) {
  //im initializing these here so I can pass them between handler and catcher
  //without resetting the values of both
  int termCount = 0; //# of term signals caught
  int totalSigCount = 0; //total # of signals caught
  catcher(argc, argv, termCount, totalSigCount);
  return 0;
}
