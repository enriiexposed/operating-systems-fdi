#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMANDS 100

// Función para ejecutar un comando y devolver el PID del proceso hijo
pid_t run_command(const char* command) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo
        execl("/bin/bash", "bash", "-c", command, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    return pid;
}

// Modo secuencial: leer líneas de un archivo y ejecutarlas una a una
void leerlineas_secuencial(char* input_file) {
    FILE* file = fopen(input_file, "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int command_num = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Eliminar el salto de línea

        printf("@@ Running command #%d: %s\n", command_num, line);
        pid_t pid = run_command(line);

        int status;
        waitpid(pid, &status, 0);

        printf("@@ Command #%d terminated (pid: %d, status: %d)\n", command_num, pid, WEXITSTATUS(status));
        command_num++;
    }
    fclose(file);
}

// Modo paralelo: ejecutar todos los comandos y esperar a que terminen
void leerlineas_paralelo(char* input_file) {
    FILE* file = fopen(input_file, "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[256];
    pid_t pids[MAX_COMMANDS];
    int command_indices[MAX_COMMANDS];
    int command_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Eliminar el salto de línea

        printf("@@ Running command #%d: %s\n", command_count, line);
        pids[command_count] = run_command(line);
        command_indices[command_count] = command_count;
        command_count++;

        if (command_count >= MAX_COMMANDS) {
            fprintf(stderr, "Demasiados comandos, límite es %d\n", MAX_COMMANDS);
            break;
        }
    }
    fclose(file);

    // Esperar a que terminen todos los procesos
    int status;
    for (int i = 0; i < command_count; i++) {
        pid_t finished_pid = waitpid(pids[i], &status, 0);
        if (finished_pid > 0) {
            for (int j = 0; j < command_count; j++) {
                if (pids[j] == finished_pid) {
                    printf("@@ Command #%d terminated (pid: %d, status: %d)\n", command_indices[j], finished_pid, WEXITSTATUS(status));
                    break;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    int opt;
    char* command = NULL;
    char* input_file = NULL;
    int paralelo = 0;

    // Procesar opciones con getopt
    while ((opt = getopt(argc, argv, "x:s:b")) != -1) {
        switch (opt) {
            case 'x':
                command = optarg;
                break;
            case 's':
                input_file = optarg;
                break;
            case 'b':
                paralelo = 1;
                break;
            default:
                fprintf(stderr, "Uso: %s -x <comando> | -s <archivo> [-b]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (command) {
        // Opción -x: Ejecutar un único comando
        pid_t pid = run_command(command);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Comando terminado con código de salida %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Comando terminado por señal %d\n", WTERMSIG(status));
        }
    } else if (input_file) {
        // Opción -s: Leer comandos de un archivo
        if (paralelo) {
            leerlineas_paralelo(input_file);
        } else {
            leerlineas_secuencial(input_file);
        }
    } else {
        fprintf(stderr, "Uso: %s -x <comando> | -s <archivo> [-b]\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
