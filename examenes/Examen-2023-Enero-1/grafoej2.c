#include <stdio.h>
#include <pthread.h>
#include <string.h>
#define MAXPROC 16

struct task {
    int valid; // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id; // identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
};

int turno = 0;

struct task tasks[MAXPROC];

int remaining_predecessors[MAXPROC] = {0};

pthread_cond_t miturno[MAXPROC];
pthread_cond_t fin_predecesores[MAXPROC];
pthread_mutex_t reduce_predecesores;
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
        printf("\n");
    }
}

void wait_for_predecesors(int id) {
    // Solo entra el proceso original (el 0, los demás se tienen que dormir a la espera de ser llamados por alguno de los predecesores)
    pthread_mutex_lock(&mtx);
    while (remaining_predecessors[id]) {
        pthread_cond_wait(&miturno[id], &mtx);
    }
    pthread_mutex_unlock(&mtx);
}

void notify_successors(int id) {
    pthread_mutex_lock(&mtx);
    // me aseguro de que
    for(int i = 0; i <MAXPROC; i++) {
        if (tasks[i].valid && tasks[id].next[i]) {
            remaining_predecessors[i]--;
            if (remaining_predecessors[i] == 0) {
                pthread_cond_signal(&miturno[i]);
            }
        }
    }
    pthread_mutex_unlock(&mtx);
}

void *task_body(void * arg) {
    int id = *(int *) arg;
    wait_for_predecesors(id); // bloqueante, el hilo espera a que sus predecesores le notifiquen su finalización
    printf("Task %d running\n", id); // cuerpo real de la tarea
    notify_successors(id); // sincronización, aviso a los sucesores de que esta tarea ha terminado
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

    char line[200];
    int length = 0;

    int nombre, numpredecesores;

    while(fgets(line, 200, file) != NULL) {
        char nextstr[100];
        char* nextptr, *token;
        
        sscanf(line, "%d,%d,%s", &nombre, &numpredecesores, &nextstr);
        tasks[nombre].valid = 1;
        tasks[nombre].id = nombre;
        remaining_predecessors[nombre] = numpredecesores;
        nextptr = nextstr;
        
        int i = 0;


        if (numpredecesores == 1) {
            long aux = strtol(nextstr);
            tasks[aux].next[nombre] = 1;
        } else {
            while(i < numpredecesores && (token = strsep(&nextptr, ",")) != NULL) {
                if (numpredecesores == 1) {
                    long aux = strtol(nextstr);
                    tasks[aux].next[nombre] = 1;
                } else {
                    tasks[atoi(token)].next[nombre] = 1;
                }
                i++;    
            }
        }

        length++;
    }

    print_graph(tasks, length);

    for (int i = 0; i<MAXPROC; i++){
        if (tasks[i].valid) {
            pthread_create(&threads[i], NULL, task_body, &tasks[i].id);
        }
    }

    for (int i = 0; i<MAXPROC; i++) {
        if (tasks[i].valid) {
            pthread_join(threads[i], NULL);
        }
    }

    fclose(file);
}