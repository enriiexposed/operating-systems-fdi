// cocinero.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define SHM_NAME "caldero_shm"
#define SEM_EMPTY "sem_empty"
#define SEM_FULL "sem_full"
#define SEM_MUTEX "sem_mutex"
#define M 5

int *caldero;
sem_t *sem_empty, *sem_full, *sem_mutex;

void limpiar_recursos() {
    sem_close(sem_empty);
    sem_close(sem_full);
    sem_close(sem_mutex);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);
    shm_unlink(SHM_NAME);
}

void manejador_senales(int signo) {
    printf("\n[COCINERO] Señal %d recibida. Limpiando recursos y saliendo...\n", signo);
    limpiar_recursos();
    exit(0);
}

void putServingsInPot() {
    printf("[COCINERO] Llenando el caldero con %d raciones.\n", M);
    *caldero = M;
    sem_post(sem_full);
}

int main() {
    signal(SIGINT, manejador_senales);
    signal(SIGTERM, manejador_senales);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("[COCINERO] Error al crear memoria compartida");
        exit(1);
    }

    if (ftruncate(shm_fd, sizeof(int)) == -1) {
        perror("[COCINERO] Error al truncar memoria compartida");
        exit(1);
    }

    caldero = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (caldero == MAP_FAILED) {
        perror("[COCINERO] Error al mapear memoria compartida");
        exit(1);
    }

    sem_empty = sem_open(SEM_EMPTY, O_CREAT, 0666, 0);
    sem_full = sem_open(SEM_FULL, O_CREAT, 0666, 0);
    sem_mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);

    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED || sem_mutex == SEM_FAILED) {
        perror("[COCINERO] Error al crear semáforos");
        limpiar_recursos();
        exit(1);
    }

    printf("[COCINERO] Recursos creados. Esperando solicitudes...\n");
    while (1) {
        sem_wait(sem_empty);
        putServingsInPot();
    }

    return 0;
}