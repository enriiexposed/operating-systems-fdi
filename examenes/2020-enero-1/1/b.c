#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int suma = 0;  // Variable global inicializada a 0
pthread_mutex_t suma_mutex = PTHREAD_MUTEX_INITIALIZER;

void* fun1(void* arg) {
    int arg1 = *((int*)arg);
    pthread_t id = pthread_self();
    printf("Hilo tipo 1 con id %lu. Mi argumento de entrada es %d\n", id, arg1);
    pthread_mutex_lock(&suma_mutex);
    suma++; // Sumar el argumento a la variable global
    printf("El número total de nproducciones es: %d\n", suma);
    pthread_mutex_unlock(&suma_mutex);
    free(arg);  // Liberar memoria del argumento
    return NULL;
}

void* fun2(void* arg) {
    pthread_t id = pthread_self();
    printf("Hilo tipo 2 con id %lu\n", id);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <M>\n", argv[0]);
        return 1;
    }
    
    int N = atoi(argv[1]);  // Número de hilos tipo 1
    int M = atoi(argv[2]);  // Número de hilos tipo 2
    
    pthread_t *threads = malloc((N + M) * sizeof(pthread_t));
    if (!threads) {
        perror("malloc");
        return 1;
    }
    
    // Crear N hilos tipo 1
    for (int i = 0; i < N; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;  // Argumento: iterador + 1
        pthread_create(&threads[i], NULL, fun1, arg);
    }
    
    // Crear M hilos tipo 2
    for (int i = 0; i < M; i++) {
        pthread_create(&threads[N + i], NULL, fun2, NULL);
    }
    
    // Esperar a que terminen todos los hilos
    for (int i = 0; i < N + M; i++) {
        pthread_join(threads[i], NULL);
    }
    
    free(threads);
    
    // Devolver el valor de suma (que es 0)
    return suma;
}