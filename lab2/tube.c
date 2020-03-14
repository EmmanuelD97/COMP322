#include <stdio.h>   /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno - number of last error */
#include <sys/wait.h> /* waitpid() */

int main(int argc, char *argv[])
{
    int pipefd[2];
    pipe(pipefd);
    pid_t  pid = fork();

    if (pid == -1)
    {
        /* Error:
         * When fork() returns -1, an error happened
         * (for example, number of processes reached the limit). */
        fprintf(stderr, "fork error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* Child process:
         When fork() returns 0, we are in the child process */

        execve(argv[1], argv + 1, NULL);
    }
    else
    {
        pid_t pid2 = fork();
        if (pid2 > 0)
        {
             fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
             fprintf(stderr, "%s: $$ = %d\n", argv[4], pid2);
        }
        else if (pid2 == 0)

        {}
        {

        }
        /* When fork() returns a positive number, we are in the parent process
         (the fork return value is the PID of the newly created child process) */
         int stat_loc;
         fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
         waitpid(pid, &stat_loc, 0);
         fprintf(stderr, "%s: $? = %d\n", argv[1], stat_loc);
    }
    return 0;
}
