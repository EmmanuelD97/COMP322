#include <stdio.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void forkNLaunch(int argc, char *argv[]) {
  pid_t  pid;
  int status;
  //int status;
  pid = fork();
  //waitpid(pid, &status, WUNTRACED);

  if (pid == 0) {
    execve(argv[1], argv + 1, NULL);
    _exit(0);  //exit child process
  }

  else if (pid > 0) {
    fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
    waitpid(pid, &status,0);

    fprintf(stderr, "%s: $? = %d \n", argv[1], status);
    exit(0);
  }

  else {
    //negative number was returned from fork() so error
    fprintf(stderr, "fork error %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  forkNLaunch(argc, argv);
  return 0;
}
