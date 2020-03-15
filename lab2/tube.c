/*
Emmanoel Dermkrdichyan lab2 Tube
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

/*
In case of a fork error this method is called which prints
an error msg and exits
*/
void forkErr() {
    fprintf(stderr, "fork error %d\n", errno);
    exit(EXIT_FAILURE);
}

void forker(int argc, char *argv[], int arg2) {
    int pipe1[2]; //pipe in and write end
    int wait1, wait2; //used for when we want to wait for forks
    int state, state2; //the state for the waitpid
    pid_t  pid, pid2; //for first and second fork

    pipe(pipe1); //creating the pipe
    pid = fork(); //first fork

    wait1 = waitpid(pid, &state, WUNTRACED); //waiting for child

    if (pid == -1) {
        forkErr(); //error while forking
    }
    else if (pid == 0) { //child
        dup2(pipe1[1], 1); 
        execve(argv[1], argv + 1, NULL); //execute first program
        _exit(0);
    }
    else { //parent
        pid2 = fork(); //second fork
        if (pid2 == -1) {
            forkErr(); //forking error
        }
        else if (pid2 > 0) {
            fprintf(stderr, "%s: $$ = %d\n", argv[1], pid); //print first childs pid in stderr
            fprintf(stderr, "%s: $$ = %d\n", argv[arg2], pid2); //print second childs pid in stderr
            wait2 = waitpid(pid2, &state2, WUNTRACED); //waiting for second child
        }
        else if (pid2 == 0) { //child 2
            dup2(pipe1[0], 0);
            execve(argv[arg2], argv + arg2, NULL); //execute second program
            _exit(0);
        }
        if (pid > 0){
            int exitStat1 = WEXITSTATUS(state); //we get the exit status of first child
            int exitStat2 = WEXITSTATUS(state2); //exit status of second child

            fprintf(stderr, "%s: $? = %d\n", argv[1], exitStat1); //we print the childrens exit status here
            fprintf(stderr, "%s: $? = %d\n", argv[arg2], exitStat2);

            close(pipe1[1]); //close both ends of the pipe
            close(pipe1[0]);
        }
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int arg2;
    /*
    This forloop is used to figure out where the second argument
    starts by comparing the arguments to a ","
    */
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i] , ",") == 0) {
            argv[i] = NULL;
            arg2 = i + 1;
        }
    }

    /*
    I created a separate method to continue the forking
    and executions of the commands so that it would be more
    modular
    */
    forker(argc, argv, arg2);
    return 0;
}
