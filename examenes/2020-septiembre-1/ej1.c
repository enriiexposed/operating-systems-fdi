#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


pthread_mutex_t m;
pthread_t threads[5];
pthread_cond_t conds[5];

int completado[5] = {0, 0, 0, 0, 0}; // A, B, C, D, E

void* funcA(void* arg) {
    pthread_mutex_lock(&m);
    // A no tiene dependencias, ejecuta inmediatamente
    printf("soy A\n");
    completado[0] = 1;
    pthread_cond_broadcast(&conds[0]); // Despertar B y C
    pthread_mutex_unlock(&m);
    return NULL;
}

void* funcB(void* arg) {
    pthread_mutex_lock(&m);
    // B espera a que A termine
    while (!completado[0]) {
        pthread_cond_wait(&conds[0], &m);
    }
    printf("soy B\n");
    completado[1] = 1;
    pthread_cond_broadcast(&conds[1]); // Despertar D
    pthread_mutex_unlock(&m);
    return NULL;
}

void* funcC(void* arg) {
    pthread_mutex_lock(&m);
    // C espera a que A termine
    while (!completado[0]) {
        pthread_cond_wait(&conds[0], &m);
    }
    printf("soy C\n");
    completado[2] = 1;
    pthread_cond_broadcast(&conds[2]); // Despertar D y E
    pthread_mutex_unlock(&m);
    return NULL;
}

void* funcD(void* arg) {
    pthread_mutex_lock(&m);
    // D espera a que B y C terminen
    while (!completado[1] || !completado[2]) {
        if (!completado[1]) pthread_cond_wait(&conds[1], &m);
        if (!completado[2]) pthread_cond_wait(&conds[2], &m);
    }
    printf("soy D\n");
    completado[3] = 1;
    pthread_cond_broadcast(&conds[3]); // Despertar E
    pthread_mutex_unlock(&m);
    return NULL;
}

void* funcE(void* arg) {
    pthread_mutex_lock(&m);
    // E espera a que D y C terminen
    while (!completado[3] || !completado[2]) {
        if (!completado[3]) pthread_cond_wait(&conds[3], &m);
        if (!completado[2]) pthread_cond_wait(&conds[2], &m);
    }
    printf("soy E\n");
    completado[4] = 1;
    pthread_mutex_unlock(&m);
    return NULL;
}

int main() {

    pthread_mutex_init(&m, NULL);
    for (int i = 0; i < 5; i++) {
        pthread_cond_init(&conds[i], NULL);
    }
    pthread_create(&threads[0], NULL, funcA, NULL);
    pthread_create(&threads[1], NULL, funcB, NULL);
    pthread_create(&threads[2], NULL, funcC, NULL);
    pthread_create(&threads[3], NULL, funcD, NULL);
    pthread_create(&threads[4], NULL, funcE, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}