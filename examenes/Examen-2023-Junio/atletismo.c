#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NTH 10


int num_dorsal = 0;
int corredores[NTH] = {-1};
int corredores_listos = 0;

int ticket = 0;       // Ticket actual asignado
int turno_actual = 0; // Ticket en turno

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t all_ready = PTHREAD_COND_INITIALIZER;
pthread_cond_t listo = PTHREAD_COND_INITIALIZER;

int dorsales[NTH] = {-1};

void change_clothes(int id) {
    printf("Runner %d is changing clothes\n", id);
}

int get_position(int id) {
    int i = 0;
    
    pthread_mutex_lock(&m);
    i = num_dorsal++;
    pthread_mutex_unlock(&m);
    return i;
}

void move_position(int id, int pos) {
    printf ("Runner %d reaches its starting position %d\n", id, pos);
    corredores[pos] = id;
}

void wait_all_ready(int id) {
    pthread_mutex_lock(&m);
    corredores_listos++;
    if (corredores_listos == NTH) {
        pthread_cond_broadcast(&all_ready);
    } else {
        while(corredores_listos != NTH) {
            pthread_cond_wait(&all_ready, &m);
        }
    }
    pthread_mutex_unlock(&m);
}

void wait_my_turn(int id, int pos) {
    pthread_mutex_lock(&m);
    while(pos != turno_actual) {
        pthread_cond_wait(&listo, &m);
    }
    pthread_mutex_unlock(&m);
}

void run(int id, int pos) {
    printf("Runner %d running from position %d\n", id, pos);
    usleep(5000);
}

void pass_on_relay(int id) {
    pthread_mutex_lock(&m);
    turno_actual++;
    pthread_cond_broadcast(&listo);
    pthread_mutex_unlock(&m);
}

void *thmain(void *arg) {
    int id = *(int*) arg;
    int pos;
    change_clothes(id);
    pos = get_position(id);
    move_position(id, pos);
    wait_all_ready(id);
    wait_my_turn(id, pos);
    run(id, pos);
    pass_on_relay(id);
    return NULL;
}

int main() {
    
    pthread_t th[NTH];

    int ids[NTH];

    for (int i = 0; i < NTH; i++) {
        ids[i] = i;
        pthread_create(&th[i], NULL, thmain, (void*) &ids[i]);
    }

    for (int i = 0; i < NTH; i++) {
        pthread_join(th[i], NULL);
    }
    
    return 0;
}
