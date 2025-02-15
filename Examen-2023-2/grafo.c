#include <stdio.h>
#include <pthread.h>
#include <string.h>
#define MAXPROC 16

struct task {
    int valid; // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id;
    int pending;
// identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
};

int turno = 0;

struct task tasks[MAXPROC];

pthread_cond_t miturno[MAXPROC];
pthread_mutex_t mtx;

pthread_t threads[MAXPROC];

void print_graph(struct task tasks[], int n)
{
    int i,j;
    for (i = 0; i < n; i++) {
        if (!tasks[i].valid) {
            continue;
        }
        printf("%d: ", tasks[i].id);

        for (j = 0; j < n; j++) {
            if (tasks[i].next[j]) {
                printf("%d ", j);
            }
        }

        for (j = 0; j < n; j++) {
            if (tasks[j].next[i]) {
                tasks[i].pending++;
            }
        }
        printf("\n"); 
    }
           
}

void wait_for_predecesors(int id) {
    pthread_mutex_lock(&mtx);
    while(tasks[id].pending > 0) {
        pthread_cond_wait(&miturno[id], &mtx);
    }
    pthread_mutex_unlock(&mtx);
}

void notify_succesors(int id) {
    pthread_mutex_lock(&mtx);
    for (int i = 0; i < MAXPROC; i++) {
        if (tasks[id].next[i]) {
            pthread_cond_signal(&miturno[i]);
        }
    }
    pthread_mutex_unlock(&mtx);
}

void *task_body(void * arg) {
    int id = *(int *) arg;
    wait_for_predecesors(id);
    // bloqueante, el hilo espera a que sus predecesores le notifiquen su finalización
    printf("Task %d running\n", id); // cuerpo real de la tarea
    int* it = tasks[id].next;
    while(it != NULL) {
        tasks[*it].pending--;
        it++;
    }
    notify_successors(id);
    // sincronización, aviso a los sucesores de que esta tarea ha terminado
    return NULL;
}


int main(int argc, char* argv[]) {
    FILE* file;
    
    if (argc != 2) {
        return -1;
    }

    file = fopen(argv[1], "r+");

    if (file == NULL) {
        printf("El archivo no existe\n");
        return -1;
    }
    
    memset(tasks, 0, sizeof(tasks));

    char line[200];
    int i = 0;

    while(fgets(line, 200, file)) {
        int id, num;
        sscanf(line, "%d,%d", &id,&num);
        printf("%d, %d\n", id, num);
        
        
        tasks[i].valid = 1;
        tasks[i].id = id;

        int j=0;
        
        char* dummy;
        char* punt = line;
        while((dummy = strsep(&punt, ","))) {
            if (j >= 2){
                int num = strtol(dummy, NULL, 10);
                printf("%d\n", num);
                tasks[num].next[i] = 1;
            }
            j++;
        }
        i++;
    }
    
    print_graph(tasks, i);

    for (int i = 0; i<MAXPROC; i++){
        pthread_create(threads[i], NULL, task_body, tasks[i].id);
        pthread_join(threads[i], NULL);
    }

    fclose(file);
}