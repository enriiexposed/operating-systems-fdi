#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 3
#define STR(arg) arg ? "PROFE" : "ESTUDIANTE"

struct persona {
    int id; 
    int tipo;
}

int espera_estuddiantes = 0, espera_profesores = 0;
int turn_est = 0, ticket_est = 0;
int turn_prof = 0, ticket_prof = 0;
pthread_mutex_t m;
pthread_cond_t espera_estudiante, espera_profesores;

void entrada()
void estancia(int id, int tipo) {
    printf("El usuario tipo")
}

void entry_lib(void* arg) {
    int tipo = *(int*) arg;

    if () {
        
    }
}


int main(int argc, char** argv) {
    FILE* f = fopen("output.in", "r");

    if (f == NULL) {
        perror("No existe el archivo\n");
        exit(1);
    }

    char buf[10];

    int pers = atoi(fscanf(f, buf, "%d"));

    pthread_t* th = malloc(sizeof(pthread_t)*pers);

    for(int i = 0; i < pers; i++) {
        fgets(buf, 2, f);
        buf[strlen(buf) - 1] = '\0';
        int tipo = atoi(buf);
        pthread_create(&th[i], NULL, entry_lib, &tipo);
    }

    free(th);
}