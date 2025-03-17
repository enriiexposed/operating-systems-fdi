#include <stdio.h>
#include <pthread.h>


struct jugador {
    int dorsal; // dorsal (único) del jugador
    int eficTiro; // entero entre 1 y 100 que determina su efectividad en el tiro
    int eficRebote; // entero entre 1 y 100 que determina su efectividad en el rebote
    int puntos; // puntos conseguidos durante el entrenamiento
};

#define NJUGADORES 4

enum estadoBalon_t {POSESION, ENAIRE};

#define MAXREBOTE 70

#define TOPEANOTACION 5

enum estadoBalon_t balon = ENAIRE; // Modela el estado actual del balón
int dificultadRebote = 0; // var. Global para indicar la dificultad del rebote actual

struct jugador jugadores[NJUGADORES] = {
    {0,30,MAXREBOTE,0}, // jugador con dorsal 0, eficTiro 30, eficRebote MAXREBOTE
    {1,80,20,0} ,
    {2,40,MAXREBOTE,0} ,
    {3,50,50,0}
};

char finished = 0;

pthread_cond_t enAire, enPosesion;
pthread_mutex_t mtx;

void rebotear(struct jugador* jug) {
    pthread_mutex_lock(&mtx);
    while(dificultadRebote < jug->eficRebote && balon != ENAIRE) {
        pthread_cond_wait(&enAire, &mtx);
    }

    balon = POSESION;
    printf("Reboteador con dorsal %d procede a correr\n", jug->dorsal);
    pthread_mutex_unlock(&mtx);
}

void tirar(struct jugador* jug) {
    pthread_mutex_lock(&mtx);
    balon = ENAIRE;
    
    int dificultadTiro = rand() % 100;
    if (dificultadTiro < jug->eficTiro) {
        jug->puntos += 2;
    }
    
    dificultadRebote = rand() % MAXREBOTE;
    if (jug->puntos >= TOPEANOTACION) {
        finished = 1;
        printf("Jugador ha llegado a los puntos\n");
    }
    printf("El jugador con dorsal %d ha tirado a canasta\n", jug->dorsal);
    pthread_cond_broadcast(&enAire);
    pthread_mutex_unlock(&mtx);
}

void correr() {
    sleep(1);
}

void* jugadorInit(void* args) {
    struct jugador* jug = (struct jugador*) args;

    /*
    printf("Caracteristicas: %d\n");
    printf("Dorsal: %d\n", jug->dorsal);
    printf("Eficiencia tiro: %d\n", jug->eficTiro);
    printf("Eficiencia rebote: %d\n", jug->eficRebote);
    printf("Puntos: %d\n", jug->puntos);
    */

    while(!finished) {
        rebotear(jug);
        correr();
        tirar(jug);
        correr();
    }

    return NULL;
}


int main(int argc, char** argv) {
    int i;
    pthread_t threads[NJUGADORES];
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&enAire, NULL);
    pthread_cond_init(&enPosesion, NULL);
    for (i = 0; i<NJUGADORES; i++) {
        pthread_create(&threads[i], NULL, jugadorInit, &jugadores[i]);
    }

    for(i = 0; i<NJUGADORES; i++) {
        pthread_join(threads[i], NULL);
        printf("Puntos del jugador con dorsal %d -> %d\n", jugadores[i].dorsal, jugadores[i].puntos);
    }

    pthread_cond_destroy(&enAire);
    pthread_cond_destroy(&enPosesion);
    pthread_mutex_destroy(&mtx);
}