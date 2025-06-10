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

int closed = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t espera_estudiante = PTHREAD_COND_INITIALIZER, espera_profesores = PTHREAD_COND_INITIALIZER;

void libreria_enter(int id, int tipo) {
    int myturn;
    
    pthread_mutex_lock(&m);
    if (tipo) {
        myturn = ticket_prof++;
        while(estudiantes + profesores >= N || myturn != turn_prof || closed) {
            wait_prof++;
            printf("User %d (%s) is waiting on the queue\n", id, STR(tipo));
            pthread_cond_wait(&espera_profesores, &m);
        }
        turn_prof++;
        profesores++;
    } else {
        myturn = ticket_est++;
        while(estudiantes + profesores >= N || wait_prof > 0 || myturn != turn_est || closed) {
            wait_est++;
            printf("User %d (%s) is waiting on the queue\n", id, STR(tipo));
            pthread_cond_wait(&espera_estudiante, &m);
        }
        turn_est++;
        estudiantes++;   
    }

    if (wait_prof > 0 && !closed) {
        wait_prof--;
        pthread_cond_broadcast(&espera_profesores);
    }

    if (wait_est > 0 && wait_prof == 0 && !closed) {
        wait_est--;
        pthread_cond_broadcast(&espera_estudiante);
    }
    pthread_mutex_unlock(&m);
}

void do_something(int id, int tipo) {
    printf("User %d (%s) is reading books for 2 seconds.\n", id, STR(tipo));
    sleep(2);
}

void libreria_exit(int id, int tipo) {
    pthread_mutex_lock(&m);
    printf("User %d is exiting the library\n", id);
    if (tipo) {
        profesores--;
    } else {
        estudiantes--;
    }

    if (wait_prof > 0 && !closed) {
        wait_prof--;
        pthread_cond_broadcast(&espera_profesores);
    }
    if (wait_est > 0 && wait_prof == 0 && !closed) {
        wait_est--;
        pthread_cond_broadcast(&espera_estudiante);
    }
    pthread_mutex_unlock(&m);
}

void estancia(void* arg) {
    struct persona* tipo = (struct persona*) arg;

    libreria_enter(tipo->id, tipo->tipo);
    do_something(tipo->id, tipo->tipo);
    libreria_exit(tipo->id, tipo->tipo);
}

// -------------------------------------------------

void cierre(void* arg) {
    while(1) {
        sleep(4);
        printf("Library is closing :(\n");
        closed = 1;
        sleep(2);
        closed = 0;
        pthread_mutex_lock(&m);
        if (wait_prof > 0 && !closed) {
            wait_prof--;
            pthread_cond_broadcast(&espera_profesores);
        } else if (wait_est > 0 && wait_prof == 0 && !closed) {
            wait_est--;
            pthread_cond_broadcast(&espera_estudiante);
        }
        pthread_mutex_unlock(&m);
        printf("Library is opening again\n");
    }
}

int main(int argc, char** argv) {
    FILE* f = fopen("in", "r");

    if (f == NULL) {
        perror("No existe el archivo\n");
        exit(1);
    }

    int pers;
    fscanf(f, "%d", &pers);
    pthread_t* th = malloc(sizeof(pthread_t) * (pers + 1));

    int id;
    struct persona personas[N];
    for(int i = 0; i < pers; i++) {
        fscanf(f, "%d", &id);
        personas[i].id = i;
        personas[i].tipo = id; 
        pthread_create(&th[i], NULL, estancia, (void*) &personas[i]);
    }

    // Hilo de la espera
    pthread_create(&th[pers], NULL, cierre, NULL);

    for (int i = 0; i<pers; i++) {
        pthread_join(th[i], NULL);
    }

    free(th);
}