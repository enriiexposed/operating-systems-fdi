#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <string.h>
#define MAX_SBUFFER_SIZE 4
#define MAX_LINE_LENGTH 256

char* shared_buffer[MAX_SBUFFER_SIZE];
int puntProd = 0, puntCons = 0;
int num_elementos = 0;

FILE* in_fd = NULL, *out_fd = NULL;

pthread_cond_t buffer_not_empty;
pthread_cond_t buffer_not_full;
pthread_mutex_t buffer_mutex;


int count_letters(const char* line) {
    int count = 0;
    char* p = line;
    while(*p != '\n') {
        count++; p++;
    }
    return count + 1;
}

void* producer () {
    char intermedio[MAX_LINE_LENGTH];

    pthread_mutex_lock(&buffer_mutex);
    while(1) {
        // ¿Puede seguir insertando elementos?
        while (num_elementos == MAX_SBUFFER_SIZE) {
            pthread_cond_wait(&buffer_not_full, &buffer_mutex);
        }
        // Si ha llegado al final del fichero, insertar NULL y salir
        if (fgets(intermedio, MAX_LINE_LENGTH, in_fd) == NULL) {
            shared_buffer[puntProd] = NULL;
            puntProd = (puntProd + 1) % MAX_SBUFFER_SIZE;
            num_elementos++;
            pthread_cond_signal(&buffer_not_empty);  // CRÍTICO: despertar al consumer
            break;
        }
        // En caso contrario, insertar línea leída
        else {
            shared_buffer[puntProd] = malloc(MAX_LINE_LENGTH);
            memcpy(shared_buffer[puntProd], intermedio, MAX_LINE_LENGTH);
            puntProd = (puntProd + 1) % MAX_SBUFFER_SIZE;
            num_elementos++;
        }
        pthread_cond_signal(&buffer_not_empty);
    }
    pthread_mutex_unlock(&buffer_mutex);
    return NULL;
}

void* consumer () {
    pthread_mutex_lock(&buffer_mutex);
    while(1) {
        while (num_elementos == 0) {
            pthread_cond_wait(&buffer_not_empty, &buffer_mutex);
        }
        if (shared_buffer[puntCons] == NULL) {
            break;
        }
        fprintf(out_fd, "%s", shared_buffer[puntCons]);
        free(shared_buffer[puntCons]);
        puntCons = (puntCons + 1) % MAX_SBUFFER_SIZE;
        num_elementos--;
        pthread_cond_signal(&buffer_not_full);
    }
    pthread_mutex_unlock(&buffer_mutex);
    return NULL;
}


int main(int argc, char *argv[]) {
    int opt;

    in_fd = stdin;
    out_fd = stdout;

    while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
        switch (opt) {
            case 'i':
                in_fd = fopen(optarg, "r");
                if (in_fd == NULL) {
                    perror("Error opening input file");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                out_fd = fopen(optarg, "w");
                if (out_fd == NULL) {
                    perror("Error opening output file");
                    exit(EXIT_FAILURE); 
                }
                break;
            case 'h':
            default:
                printf("Usage: %s [-i input_file] [-o output_file]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    pthread_t threads[2];
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_cond_init(&buffer_not_empty, NULL);
    pthread_cond_init(&buffer_not_full, NULL);
    pthread_create(&threads[0], NULL, producer, &in_fd);
    pthread_create(&threads[1], NULL, consumer, &out_fd);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    if (in_fd != stdin) fclose(in_fd);
    if (out_fd != stdout) fclose(out_fd);
    return 0;
}