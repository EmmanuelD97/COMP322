#include <stdio.h>   /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <sys/times.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno */

void reportStatus(int pid, int retVal, struct tms timer) {
  /*The if statement here makes it so that we can use the same method for both
  the parent and the child without reusing code
  */
  if (pid == 0) { //child side
    printf("START: %ld\n", time(NULL));
    printf("PPID: %d, PID: %d\n", getppid(), getpid());
  }
  else if (pid > 1) { //parent side
    printf("PPID: %d, PID: %d, ", getppid(), getpid());
    printf("CPID: %d, RETVAL: %d\n", pid, retVal);

    printf("USER: %ld, SYS:  %ld\n", timer.tms_utime, timer.tms_stime);
    printf("CUSER:%ld, CSYS: %ld\n", timer.tms_cutime, timer.tms_cstime);
    printf("STOP: %ld\n", time(NULL));
  }
}

int main(void)
{
  pid_t  pid;
  int status;
  struct tms timer;
  clock_t sinceboot;
  clock_t times(struct tms *timer);

  sinceboot = times(&timer); //setting time
  pid = fork(); //creating fork

  if (pid == 0) {
    reportStatus(pid, WEXITSTATUS(status), timer); //calling the print method
    sleep(3);  //putting the process to sleep to change time between start and stop
    _exit(0);  //exit child process
  }

  else if (pid > 0) {
    waitpid(pid, &status, WUNTRACED); //waits for the child to finish before continuing
    //printing method for child and parent
    reportStatus(pid, WEXITSTATUS(status), timer);
    exit(0);
  }

  else {
    //negative number was returned from fork() so error
    fprintf(stderr, "can't fork, error %d\n", errno);
    exit(EXIT_FAILURE);
  }
  return 0;
}
