// salvajes.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define SHM_NAME "caldero_shm"
#define SEM_EMPTY "sem_empty"
#define SEM_FULL "sem_full"
#define SEM_MUTEX "sem_mutex"
#define NUMITER 10

int *caldero;
sem_t *sem_empty, *sem_full, *sem_mutex;

void getServingFromPot(int id) {
    sem_wait(sem_mutex);
    if (*caldero == 0) {
        printf("[SALVAJE %d] El caldero está vacío. Avisando al cocinero...\n", id);
        sem_post(sem_empty);
        sem_wait(sem_full);
    }
    (*caldero)--;
    printf("[SALVAJE %d] Ha tomado una ración. Raciones restantes: %d\n", id, *caldero);
    sem_post(sem_mutex);
}

void eat(int id) {
    printf("[SALVAJE %d] Está comiendo.\n", id);
    sleep(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <id_salvaje>\n", argv[0]);
        exit(1);
    }

    int id = getpid(); // Suponemos que el pid que se asigna es único

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("[SALVAJE] Error al abrir memoria compartida. Asegúrese de que el cocinero está ejecutándose");
        exit(1);
    }

    caldero = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (caldero == MAP_FAILED) {
        perror("[SALVAJE] Error al mapear memoria compartida");
        exit(1);
    }

    sem_empty = sem_open(SEM_EMPTY, 0);
    sem_full = sem_open(SEM_FULL, 0);
    sem_mutex = sem_open(SEM_MUTEX, 0);

    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED || sem_mutex == SEM_FAILED) {
        perror("[SALVAJE] Error al abrir semáforos");
        exit(1);
    }

    for (int i = 0; i < NUMITER; i++) {
        getServingFromPot(id);
        eat(id);
    }

    printf("[SALVAJE %d] Ha terminado.\n", id);
    return 0;
}