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
    int wait1 = -1, wait2 = -1;
    pipe(pipefd);
    int posSecondArg;
    int i = 0;
    int stat_loc;
    int stat_loc2;
    int childFlag = 1;

    while(i < argc)
    {
        if(strcmp(argv[i] , ",") == 0)
        {
            argv[i] = NULL;
            posSecondArg = i + 1;
            break;
        }

        i++;
    }


    pid_t  pid = fork();
    wait1 = waitpid(pid, &stat_loc, WUNTRACED);

    if (pid == -1) {
        fprintf(stderr, "fork error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0 && childFlag == 1) { //child
        dup2(pipefd[1], 1);

        execve(argv[1], argv + 1, NULL);
        //printf("this is the childFlag %d\n", childFlag);
        _exit(0);
    }
    else if (pid > 0) { //parent
        //childFlag = 2;
        //printf("this is the childFlag %d\n", childFlag);
        pid_t pid2 = fork();
        if (childFlag == 1) {
            fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
            fprintf(stderr, "%s: $$ = %d\n", argv[4], pid2);
        }
        childFlag = 2;

        //fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
        //fprintf(stderr, "%s: $$ = %d\n", argv[4], pid2);
        //wait2 = waitpid(pid2, &stat_loc2, WUNTRACED);

        if (pid2 == -1) {
            fprintf(stderr, "fork error %d\n", errno);
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0 && childFlag == 2) { //child 2
            dup2(pipefd[0], 0);
            execve(argv[posSecondArg], argv + posSecondArg, NULL);
            //printf("this is the childFlag %d\n", childFlag);
            _exit(0);
        }
        else if (pid2 > 0){

            int exitStat1 = WEXITSTATUS(stat_loc);
            int exitStat2 = WEXITSTATUS(stat_loc2);
            fprintf(stderr, "%s: $? = %d\n", argv[1], exitStat1);
            fprintf(stderr, "%s: $? = %d\n", argv[posSecondArg], exitStat2);
            //exit(0);
        }

    //fprintf(stderr, "this\n");
        exit(0);
    }
    return 0;
}
