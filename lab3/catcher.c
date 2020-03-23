/*
Emmanoel Dermkrdichyan lab3 catcher
*/

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>

int terminator = 0; //term signal counter
static const char signals [38][16] = {"SIGABRT", "SIGALRM", "SIGBUS", "SIGCHLD", "SIGCLD", "SIGCONT", "SIGEMT", "SIGFPE", "SIGHUP"
                                  "SIGILL", "SIGINFO", "SIGINT", "SIGIO", "SIGIOT", "SIGKILL", "SIGLOST", "SIGPIPE", "SIGPOLL"
                                  "SIGPROF", "SIGPWR", "SIGQUIT", "SIGSEGV", "SIGSTKFLT", "SIGSTOP", "SIGTSTP", "SIGSYS"
                                  "SIGTERM", "SIGTRAP", "SIGTTIN", "SIGTTOU", "SIGUNUSED", "SIGURG", "SIGUSR1", "SIGUSR2"
                                  "SIGVTALRM", "SIGXCPU", "SIGXFSZ", "SIGWINCH"};
static const char signalInp[33][11] = {"HUP", "INT", "QUIT", "ILL", "TRAP", "ABRT", "BUS", "FPE", "KILL", "USR1", "SEGV", "USR2", "PIPE", "ALRM", "TERM", "STKFLT", "CHLD", "CONT", "STOP", "TSTP", "TTIN", "TTOU", "URG", "XCPU", "XFSZ", "VTALRM", "PROF", "WINCH", "IO", "PWR", "SYS", "IOT", "POLL"};

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
    for (int j = 0; j < 33; j++) {
      if (strcmp(signalInp[j], argv[i]) == 0) {
        indexToCatch[count] = j;
        count++;
      }
    }
  }
}

void handler(int sig) {
  int whichSig = 0;

}

int hasNonSig(int argc, char **argv) {
  if (argv[argc - 1][0] == '>' || argv[argc - 1][0] == '|') {
    return argc - 1;
  }
  else {
    return argc;
  }
}

void catcher(int argc, char **argv, int termCount, int totalSigCount) {
  int nonSig = 0;
  int argCount = argc;
  printPID();


  nonSig = hasNonSig(argc, argv);
  argCount = argCount - nonSig;

  for (int i = 1; i < argCount; i++) {
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
  int numberOfSigArgs = (hasNonSig(argc, argv) - 1);
  int indexToCatch[numberOfSigArgs];

  whatToCatch(argc, argv, numberOfSigArgs, indexToCatch);

  catcher(argc, argv, termCount, totalSigCount);
  return 0;
}
