#include <stdio.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int forkNLaunch(int argc, char *argv[]) {
  pid_t  pid;

  pid = fork();
  waitpid(pid, &status, WUNTRACED);

  if (pid == 0) {
    execve(argv[1], argv + 1, NULL);
    _exit(0);  //exit child process
  }

  else if (pid > 0) {
    reportStatus(pid, WEXITSTATUS(status), timer); //print method for parent and child
    exit(0);
  }

  else {
    //negative number was returned from fork() so error
    fprintf(stderr, "can't fork, error %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {

}
