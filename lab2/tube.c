#include <stdio.h>   /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno - number of last error */
#include <sys/wait.h> /* waitpid() */
#include <string.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    int pipefd2[2];
    pipe(pipefd);
    int posSecondArg;
    int i = 0;
    int stat_loc;
    int stat_loc2;
    int childFlag = 1;

    pid_t  pid = fork();
    waitpid(pid, &stat_loc, WUNTRACED);

    if (pid == -1) {
        fprintf(stderr, "fork error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0 && childFlag == 1) { //child
        execve(argv[1], argv + 1, NULL);
        printf("this is the childFlag %d\n", childFlag);
        _exit(0);
    }
    else if (pid > 0) { //parent
        childFlag = 2;
        printf("this is the childFlag %d\n", childFlag);
        pid_t pid2 = fork();
        fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
        fprintf(stderr, "%s: $$ = %d\n", argv[4], pid2);
        waitpid(pid2, &stat_loc2, WUNTRACED);

        if (pid2 == -1) {
            fprintf(stderr, "fork error %d\n", errno);
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0 && childFlag == 2) { //child 2
            execve(argv[4], argv + 1, NULL);
            printf("this is the childFlag %d\n", childFlag);
            _exit(0);
        }
        else if (pid2 > 0){
            fprintf(stderr, "%s: $$ = %d\n", argv[1], stat_loc);
            fprintf(stderr, "%s: $$ = %d\n", argv[4], stat_loc2);
        }

    //fprintf(stderr, "this\n");
        exit(0);
    }
    return 0;
}
