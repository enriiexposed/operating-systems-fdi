#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NTH 10

pthread_mutex_t m;
pthread_cond_t espera_listos, espera_orden;
pthread_t th[NTH];

int ticket = 0;

int position = 0;

int corredores_listos = 0;

void change_clothes(int *id) {
    printf("Runner %d is changing clothes\n", *id);
}

int get_position(int *id) {
    int myturn;
    pthread_mutex_lock(&m);
    myturn = position++;
    pthread_mutex_unlock(&m);
}

void move_position(int *id, int pos) {
    printf("Runner %d reaches its starting position %d\n", *id, pos);
}

void wait_all_ready(int *id) {
    pthread_mutex_lock(&m);
    corredores_listos++;
    while(corredores_listos != NTH) {
        pthread_cond_wait(&espera_listos, &m);
    }
    pthread_cond_broadcast(&espera_listos);
    pthread_mutex_unlock(&m);
}

void wait_my_turn(int *id, int pos) {
    int myturn;
    pthread_mutex_lock(&m);
    myturn = ticket++;
    while (myturn != pos) {
        pthread_cond_wait(&espera_orden, &m);
    }   
    pthread_cond_broadcast(&espera_orden);
    pthread_mutex_unlock(&m);
}

void *thmain(void *arg)
{
    int id = (int)(long long)arg;
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

int main(int argc, char ** argv) {
    pthread_attr_t attr;
    int j;
	pthread_attr_t attr;
    pthread_attr_init(&attr);
    for (j = 0; j < NTH; j++)
	{
		if (pthread_create(&th[j], &attr, thmain, &j) != 0)
		{
			perror("Error creating thread");
			exit(EXIT_FAILURE);
		}
	}

    for (j = 0; j < NTH; j++)
	{
		pthread_join(th[j], NULL);
	}

    return 0;
}   