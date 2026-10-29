// file: onepipe.cpp
// author: Jonathan Martinez
// date: 10/28/2026
// purpose: CS3377.504
// description:
// this program executes "ls -ltr | grep 3376", by dividing the two command
// among the child and parent process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv){
    int status;
    int childpid;
    char *ls_args[] = {(char *)"ls", (char *)"-ltr", NULL};
    char *grep_args[] = {(char *)"grep", (char *)"3377", NULL};

    // create one pipe to send the output of "ls" process to "grep" process
    int pipes[2];
    pipe(pipes);

    // fork the first child (to execute cat)
    if((childpid = fork()) == -1){
        perror("Error creating a child process");
    exit(1);
    }
    
    if (childpid == 0) {
        // replace cat's stdout with write part of 1st pipe
        dup2(pipes[1], 1);
        // close all pipes (very important!); end we're using was safely copied
            close(pipes[0]);
            close(pipes[1]);
        execvp(*ls_args, ls_args);
            exit(0);
    }
    else {
        // replace grep's stdin with read end of 1st pipe
        dup2(pipes[0], 0);
            close(pipes[0]);
            close(pipes[1]);
        execvp(*grep_args, grep_args);
    }
    return(0);
}
