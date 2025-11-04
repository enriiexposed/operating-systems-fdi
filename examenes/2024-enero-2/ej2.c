#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

#define NTH 10

pthread_t th[NTH];
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int fd1;

void* func(void* arg) {
    int num = *(int *) arg; // Obtener el número del argumento
    char c = '0' + num; // Asignar un carácter basado en el índice
    pthread_mutex_lock(&m);
    lseek(fd1, num * 5, SEEK_SET); // Mover el puntero del archivo
    for (int i = 0; i < 5; i++) {
        write(fd1, &c, 1); // Escribir el carácter en el archivo
    }
    pthread_mutex_unlock(&m);
    free(arg); // Liberar la memoria asignada
    return NULL; // Terminar el hilo
}

int main(void) {
    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd1 < 0) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < NTH; i++) {
        int* arg = malloc(sizeof(int)); // Asignar memoria para el argumento
        *arg = i; // Asignar el valor de i
        pthread_create(&th[i], NULL, func, arg); // Crear el hilo
    }

    for (int i = 0; i < NTH; i++) {
        pthread_join(th[i], NULL); // Esperar a que el hilo termine
    }

    close(fd1); // Cerrar el archivo
    return 0; // Terminar el programa
}