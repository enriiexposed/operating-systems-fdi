#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAXITEMS 10

int suma = 0;  // Variable global inicializada a 0

int buffer[MAXITEMS];

int punt_prod = 0;
int punt_cons = 0;

int nproducciones = 0;
int nextracciones = 0;

pthread_mutex_t suma_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t lleno = PTHREAD_COND_INITIALIZER;
pthread_cond_t vacio = PTHREAD_COND_INITIALIZER;

// productor
void* fun1(void* arg) {
    int arg1 = *((int*)arg);
    pthread_t id = pthread_self();
    
    while (1) {
        pthread_mutex_lock(&suma_mutex);
        
        // Paso 1.1: Verificar si el buffer está lleno: (punt_prod + 1) % MAXITEMS == punt_cons
        // Si está lleno, esperar a que haya espacio (señal 'vacio')
        while ((punt_prod + 1) % MAXITEMS == punt_cons && nproducciones < MAXITEMS) {
            pthread_cond_wait(&vacio, &suma_mutex);
        }
        
        // Paso 1.2: Verificar si ya alcanzamos el límite de producciones
        if (nproducciones >= MAXITEMS) {
            pthread_mutex_unlock(&suma_mutex);
            break;
        }
        
        // Paso 1.3: Insertar el valor recibido como parámetro (arg1) en el buffer
        buffer[punt_prod] = arg1;
        punt_prod = (punt_prod + 1) % MAXITEMS;
        nproducciones++;
        
        // Paso 1.4: Mostrar mensaje por stderr con el formato especificado
        printf("Productor %lu, he añadido un item %d. El número total de producciones es %d\n", 
                (unsigned long)id, arg1, nproducciones);
        
        // Paso 1.5: Señalar que hay elementos en el buffer (para consumidores)
        pthread_cond_broadcast(&lleno);
        pthread_mutex_unlock(&suma_mutex);
        
        // Paso 1.6: Esperar un tiempo aleatorio entre 1 y 5 segundos
        sleep((rand() % 5) + 1);
    }
    free(arg);  // Liberar memoria del argumento
    return NULL;
}


// consumidor
void* fun2(void* arg) {
    pthread_t id = pthread_self();
    int* suma_local = malloc(sizeof(int));
    *suma_local = 0;
    
    while (1) {
        pthread_mutex_lock(&suma_mutex);
        
        // Paso 2.1: Verificar si el buffer está vacío: punt_prod == punt_cons
        // Si está vacío, esperar a que haya elementos (señal 'lleno')
        while (punt_prod == punt_cons && nextracciones < MAXITEMS) {
            pthread_cond_wait(&lleno, &suma_mutex);
        }
        
        // Paso 2.2: Verificar si ya alcanzamos el límite de extracciones
        if (nextracciones >= MAXITEMS) {
            pthread_mutex_unlock(&suma_mutex);
            break;
        }
        
        // Paso 2.3: Extraer el elemento del buffer siguiendo el orden de producción
        int valor = buffer[punt_cons];
        punt_cons = (punt_cons + 1) % MAXITEMS;
        nextracciones++;
        
        // Paso 2.4: Actualizar la variable global suma sumando el valor extraído
        suma += valor;
        *suma_local = suma;
        
        // Paso 2.5: Mostrar mensaje por stderr con el formato especificado
        fprintf(stderr, "Consumidor %lu, valor extraído %d, acumulado %d, extracciones %d\n", 
                (unsigned long)id, valor, suma, nextracciones);
        
        // Paso 2.6: Señalar que hay espacio en el buffer (para productores)
        pthread_cond_broadcast(&vacio);
        pthread_mutex_unlock(&suma_mutex);
    }

    // Paso 2.7: Devolver el valor acumulado final de suma
    return suma_local;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <M>\n", argv[0]);
        return 1;
    }
    
    // Paso 3.1: Inicializar el generador de números aleatorios
    srand(time(NULL));
    
    int N = atoi(argv[1]);  // Número de hilos productores
    int M = atoi(argv[2]);  // Número de hilos consumidores
    
    pthread_t *threads = malloc((N + M) * sizeof(pthread_t));
    if (!threads) {
        perror("malloc");
        return 1;
    }
    
    // Paso 3.2: Crear N hilos productores (fun1)
    for (int i = 0; i < N; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;  // Argumento: iterador + 1
        pthread_create(&threads[i], NULL, fun1, arg);
    }
    
    // Paso 3.3: Crear M hilos consumidores (fun2)
    for (int i = 0; i < M; i++) {
        pthread_create(&threads[N + i], NULL, fun2, NULL);
    }
    
    // Paso 3.4: Esperar a que terminen todos los hilos productores
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Paso 3.5: Esperar a que terminen todos los hilos consumidores
    // y mostrar el valor devuelto por cada uno
    for (int i = 0; i < M; i++) {
        void* retval;
        pthread_join(threads[N + i], &retval);
        int* suma_consumidor = (int*)retval;
        fprintf(stderr, "El consumidor %d devolvió el valor: %d\n", i + 1, *suma_consumidor);
        free(suma_consumidor);
    }
    
    free(threads);
    
    // Paso 3.6: Devolver el valor final de suma
    return suma;
}