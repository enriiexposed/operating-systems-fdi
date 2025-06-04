#include <stdio.h>
#include <sys/types.h>
int globalVar;
void main() {
    int localVar=3;     
    pid_t pid;
    globalVar=10;
    printf("I am the original process. My PID is %d\n", getpid());
    fflush(NULL);
    pid = fork();
    if (pid ==-1) {
        perror("Can’t fork()\n");
        exit(-1);
    }
    if (pid == 0 ) {
        // Child process
        globalVar = globalVar + 5;
        localVar = localVar + 5;
    }
    else {
        // Parent process
        wait(NULL);
        globalVar = globalVar + 10;
        localVar = localVar + 10;
    }
    printf("I am the process with PID %d. Mi parent is %d Global: %d Local %d\n", getpid(), getppid(),globalVar, localVar );
}