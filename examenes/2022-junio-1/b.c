#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HILOS 20

#define N 5

int normal_waiting = 0, normal_ticket = 0, normal_turn = 0;

int vip_waiting = 0, vip_ticket = 0, vip_turn = 0;

int inside = 0;

struct args {
    int id;
    int isvip;
};

pthread_t threads[HILOS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t espera_vip = PTHREAD_COND_INITIALIZER;
pthread_cond_t espera_normal = PTHREAD_COND_INITIALIZER;

void dance() {
    sleep((rand() % 3) + 1);
}

void enter_vip_client(int id, int isvip) {
    int turn;
    pthread_mutex_lock(&mutex);
    turn = vip_ticket++;
    // Mientras que la sala este llena o bien no sea mi turno, me espero
    while (inside >= N || turn != vip_turn) {
        // Me añado en la espera
        vip_waiting++;
        pthread_cond_wait(&espera_vip, &mutex);
        // Me salgo de la espera y veo si puedo salir
        vip_waiting--;
    }
    // Estoy dentro, paso al siguiente de la cola de vips
    printf("VIP client %d entered\n", id);
    vip_turn++;
    inside++;
    pthread_mutex_unlock(&mutex);
}

void enter_normal_client(int id, int isvip) {
    int turn;
    pthread_mutex_lock(&mutex);
    // cojo turno
    turn = normal_ticket++;
    // Mientras que la sala este llena o haya vips esperando o no sea mi turno, me espero
    while (inside >= N || vip_waiting > 0 || turn != normal_turn) {
        // Me añado en la espera
        normal_waiting++;
        pthread_cond_wait(&espera_normal, &mutex);
        // Me salgo de la espera y veo si puedo salir
        normal_waiting--;
    }
    // Estoy dentro, paso al siguiente de la cola de normales
    printf("Normal client %d entered\n", id);
    normal_turn++;
    inside++;
    pthread_mutex_unlock(&mutex);
}

void exit_client(int id, int isvip) {
    pthread_mutex_lock(&mutex);
    // me voy
    printf("Client %d exited\n", id);
    inside--;
    // si hay vips esperando, les doy paso
    if (vip_waiting > 0) {
        pthread_cond_broadcast(&espera_vip);
    } 
    // si no, aviso a los normales para que entren
    else if (normal_waiting > 0) {
        pthread_cond_broadcast(&espera_normal);
    }
    pthread_mutex_unlock(&mutex);
}

void* client(void* arg) {
    struct args* a = (struct args*)arg;
    pthread_mutex_lock(&mutex);
    printf("Client %d, VIP status: %d\n", a->id, a->isvip);
    pthread_mutex_unlock(&mutex);
    if (a->isvip) {
        enter_vip_client(a->id, a->isvip);
    } else {
        enter_normal_client(a->id, a->isvip);
    }
    dance();
    exit_client(a->id, a->isvip);
}

int main()  {
    struct args *a[HILOS];
    for (int i = 0; i < HILOS; i++) {
        a[i] = malloc(sizeof(struct args));
        a[i]->id = i + 1;
        a[i]->isvip = (rand() % 2); // Randomly assign VIP status
        pthread_create(&threads[i], NULL, client, a[i]);
    }

    for (int i = 0; i < HILOS; i++) {
        pthread_join(threads[i], NULL);
    }
}