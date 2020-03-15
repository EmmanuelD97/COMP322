/*
Emmanoel Dermkrdichyan lab2 launch
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>

void forkNLaunch(int argc, char *argv[]) {
  pid_t  pid;
  int status, exitStat;
  pid = fork();
  if (pid == 0) { //this is the child
    execve(argv[1], argv + 1, NULL); //child executes the program
    _exit(0);
  }

  else if (pid > 0) { //this is the parent
    fprintf(stderr, "%s: $$ = %d\n", argv[1], pid); //parent prints the pid of child on stderr
    waitpid(pid, &status,0);

    exitStat = WEXITSTATUS(status);

    fprintf(stderr, "%s: $? = %d \n", argv[1], exitStat); //parent prints the exit status of child on stderr
    exit(0);
  }

  else { //there was an issue with the fork
    fprintf(stderr, "fork error %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  forkNLaunch(argc, argv); //created another method for modularity
  return 0;
}
