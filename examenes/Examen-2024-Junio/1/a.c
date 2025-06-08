#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 3
#define STR(arg) arg ? "PROFE" : "ESTUDIANTE"

struct persona {
    int id;
    int tipo;
};

int estudiantes = 0, profesores = 0;
int wait_est = 0, wait_prof = 0;
int turn_est = 0, ticket_est = 0;
int turn_prof = 0, ticket_prof = 0;
pthread_mutex_t m;
pthread_cond_t espera_estudiante, espera_profesores;
sem_t lib;

void estancia(void* arg) {
    struct persona* tipo = (struct persona*) arg;

    
    do_something(tipo->id, tipo->tipo);
    libreria_exit(tipo->tipo);
}

void libreria_enter(int tipo) {
    int myturn;
    
    pthread_mutex_lock(&m);
    if (tipo) {
        myturn = ticket_prof++;
        while(estudiantes + profesores >= N || myturn != ticket_prof) {
            wait_prof++;
            pthread_cond_wait(&espera_profesores, &m);
        }
        profesores++;
    } else {
        myturn = ticket_est++;
        while(estudiantes + profesores >= N || wait_prof > 0 || myturn != ticket_est) {
            wait_est++;
            pthread_cond_Wait(&espera_estudiante);
        }
        estudiantes++;
    }
    pthread_mutex_unlock(&m);
}

void do_something(int id, int tipo) {
    printf("El %s entro a la biblioteca", STR(tipo));
    sleep(2);
}

void libreria_exit(int tipo) {
    pthread_mutex_lock(&m);
    if (tipo) {
        profesores--;
        ticket_prof++;
    } else {
        estudiantes--;
        ticket_est++;
    }
    if (wait_prof) {
        pthread_cond_broadcast(&espera_profesores);
    } else {
        pthread_cond_broadcast(&espera_estudiante);
    }
    pthread_mutex_unlock(&m);
}


int main(int argc, char** argv) {
    FILE* f = fopen("output.in", "r");

    if (f == NULL) {
        perror("No existe el archivo\n");
        exit(1);
    }

    char buf[10];

    int pers = atoi(fscanf(f, buf, "%d"));

    pthread_t* th = malloc(sizeof(pthread_t) * pers);

    sem_init(&lib, 0, N);

    for(int i = 0; i < pers; i++) {
        fgets(buf, 2, f);
        buf[strlen(buf) - 1] = '\0';
        int tipo = atoi(buf);
        pthread_create(&th[i], NULL, estancia, &tipo);
    }

    free(th);
}