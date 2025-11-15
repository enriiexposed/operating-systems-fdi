#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int run_command (char *command) {
    // Child process
    execl("/bin/sh", "sh", "-c", command, (char *)NULL);
    // Aqui ha ocurrido un error
    return -1;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: <programa> <comando>\n");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        if (run_command(argv[1]) == -1) {
            perror("run_command failed");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Proceso hijo creado con PID: %d\n", pid);
        return pid;
    }
    
    int command = run_command(argv[1]);

    return 0;
}