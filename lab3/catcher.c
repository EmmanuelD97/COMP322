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

/*
SIGTERM counter, there was no way to increment in the handler so this was the best option.
*/
int terminator = 0;

/*
Static constant array used to keep track of all possible signals we are responsible for.
The reason its global is because we need to access it from multiple places and we it is constant.
*/
static const char signalInp[27][6] = {"HUP","INT","QUIT","ILL","TRAP","ABRT","IOT"
                                    ,"BUS","FPE","USR1","SEGV","USR2","PIPE","ALRM"
                                    ,"TERM","STKFLT","CHLD","CONT","TSTP","TTIN"
                                    ,"TTOU","URG","XCPU","XFSZ","VTALRM","PROF","WINCH"};

void printPID() {
  /*
  Simple function used to print the pid.
  */
  int pid = getpid();
  fprintf(stderr, "catcher: $$ = %d\n", pid);
}

void printTotalCT(int signalsCaught) {
  /*
  Final print statement which says the number of signals caught.
  */
  fprintf(stderr, "catcher: Total signals count = %d\n", signalsCaught);
}

void handler(int sig) {
  /*
  Prints what signal was caught and at what time.
  */
  printf("SIG%s caught at %ld\n", signalInp[sig - 1],time(NULL));

  /*
  Signal received is SIGTERM so we increment terminate counter.
  */
  if(sig == SIGTERM) {
    terminator++;
  }
  /*
  Resetting the term counter until we have 3 consequtive terms.
  */
  else if(sig != SIGTERM) {
    terminator = 0;
  }
}

void registerSign(int argc, char **argv) {
  /*
  Goes through every argument and matches it with signals.
  */
  for (int i = 1; i < argc; i++) {
    for (int j = 0; j < 27; j++) {
      /*
      We use this if statement to make sure that the signal is one we have to catch.
      If the signal matches the argument then we register it.
      */
      if (strcmp(argv[i],signalInp[j]) == 0) {
        signal(j + 1, handler);
        /*
        If signal() returns an error we print an error
        message.
        */
        if (signal(j + 1, handler) == SIG_ERR) {
          fprintf(stderr, "signal error\n");
        }
      }
    }
  }
}

void catcher(int argc, char **argv) {
  int signalsCaught = 0;
  printPID();

  /*
  Registering the signals for the first time.
  */
  registerSign(argc, argv);

  /*
  We pause until the program receives 3 consecutive terminate signals.
  */
  do {
    pause();
    signalsCaught++;

  } while (terminator != 3);

  /*
  If we get to this point it means that we have received 3 consecutive
  term signals therefore we can register the signals again and then terminate.
  */
  registerSign(argc, argv);

  /*
  Printing the final count of signals count.
  */
  printTotalCT(signalsCaught);

  /*
  Received 3 consecutive terminate signals so we exit successfully.
  */
  exit(EXIT_SUCCESS);

}

int main(int argc, char **argv) {
  /*
  Catcher takes care of main logic, it brings all the functions together.
  */
  catcher(argc, argv);
  return 0;
}
