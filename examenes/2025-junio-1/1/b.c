#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


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
        sigset_t newmask;
        
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGINT);
        sigaddset(&newmask, SIGTERM);
        sigprocmask(SIG_BLOCK, &newmask, NULL);

        printf("Proceso hijo creado con PID: %d\n", pid);
        
        int status;
        waitpid(pid, &status, 0);
        
        // Verificar cómo terminó el proceso hijo
        if (WIFEXITED(status)) {
            printf("El proceso hijo finalizó normalmente con código: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("El proceso hijo fue terminado por la señal: %d\n", WTERMSIG(status));
        }
    }

    return 0;
}