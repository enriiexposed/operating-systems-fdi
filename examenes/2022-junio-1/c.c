#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int N = 0;  // Capacidad máxima de la sala



int normal_waiting = 0, normal_ticket = 0, normal_turn = 0;

int vip_waiting = 0, vip_ticket = 0, vip_turn = 0;

int inside = 0;

struct args {
    int id;
    char isvip;
};

pthread_t *threads;

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

int main(int argc, char *argv[])  {
    // Paso 2.1: Abrir el archivo in.txt
    FILE *fp;
    if ((fp = fopen("in.txt", "r")) == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    // Paso 2.3: Leer el número total de clientes (primera línea)
    int num_clientes;
    if (fscanf(fp, "%d", &num_clientes) != 1) {
        fprintf(stderr, "Error al leer el número de clientes\n");
        fclose(fp);
        return -1;
    }

    // Paso 2.4: Asignar N (capacidad de la sala) - puede ser igual al número de clientes o un valor fijo
    // Aquí asumo que N es la capacidad máxima (puedes ajustarlo según necesites)
    N = 2;  // Capacidad de la sala (ajusta según necesites)

    // Paso 2.5: Asignar memoria dinámica para los hilos
    threads = malloc(num_clientes * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Error al asignar memoria para hilos");
        fclose(fp);
        return -1;
    }

    // Paso 2.6: Asignar memoria para los argumentos de cada cliente (array de structs)
    struct args *argumentos = malloc(num_clientes * sizeof(struct args));
    if (argumentos == NULL) {
        perror("Error al asignar memoria para argumentos");
        free(threads);
        fclose(fp);
        return -1;
    }

    // Paso 2.7: Leer el estado VIP de cada cliente y crear los hilos
    for (int i = 0; i < num_clientes; i++) {
        argumentos[i].id = i + 1;
        
        // Leer el valor isvip (0 o 1) de cada cliente
        int isvip_value;
        if (fscanf(fp, "%d", &isvip_value) != 1) {
            fprintf(stderr, "Error al leer el estado VIP del cliente %d\n", i + 1);
            free(argumentos);
            free(threads);
            fclose(fp);
            return -1;
        }
        
        argumentos[i].isvip = (char)isvip_value;
        
        // Crear el hilo para este cliente
        pthread_create(&threads[i], NULL, client, &argumentos[i]);
    }

    // Paso 2.8: Esperar a que terminen todos los hilos
    for (int i = 0; i < num_clientes; i++) {
        pthread_join(threads[i], NULL);
    }

    // Paso 2.9: Liberar memoria y cerrar el archivo
    free(argumentos);
    free(threads);
    fclose(fp);

    return 0;
}