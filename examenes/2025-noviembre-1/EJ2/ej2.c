#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <err.h>

#define NTH 10

int reserva_posicion = 0;

int corredores_listos = 0;

int ticket = 0;

pthread_mutex_t m;
pthread_cond_t espera_listo = PTHREAD_COND_INITIALIZER, espera_turno = PTHREAD_COND_INITIALIZER;

void change_clothes(int id)
{
	printf("Runner %d is changing clothes\n", id);
	sleep(1 + (rand() % 3));
}

void move_position(int id, int pos)
{
	sleep(1 + (rand() % (1 + pos)));
	printf("Runner %d reaches its starting position %d\n", id, pos);
}

void run(int id, int pos)
{
	printf("Runner %d running from position %d\n", id, pos);
	sleep(1 + (rand() % 3));
}

/* apartado b */
int get_position(int id)
{
	int pos;

	pthread_mutex_lock(&m);
	pos = reserva_posicion++;
	pthread_mutex_unlock(&m);

	return pos;
}

/* apartado c */
void wait_all_ready(int id)
{
	pthread_mutex_lock(&m);
	corredores_listos++;
	if (corredores_listos == NTH) {
        pthread_cond_broadcast(&espera_listo);
    } else {
        while(corredores_listos != NTH) {
            pthread_cond_wait(&espera_listo, &m);
        }
    }
	pthread_mutex_unlock(&m);
}

/* apartado d */
void wait_my_turn(int id, int pos)
{
	// Espero a que mi ticket coincida con mi numero de posicion que tengo
	pthread_mutex_lock(&m);
	while (ticket != pos) {
		pthread_cond_wait(&espera_turno, &m);
	}
	pthread_mutex_unlock(&m);
}

/* apartado e */
void pass_on_relay(int id)
{
	pthread_mutex_lock(&m);
    ticket++;
    pthread_cond_broadcast(&espera_turno);
    pthread_mutex_unlock(&m);
}

void *thmain(void *arg)
{
	int id = (int) (long long) arg;
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

int main(int argc, char *argv[])
{
	int i;

	/* Apartado a */

	pthread_t thid[10];
	for (int i = 0; i < NTH; i++) {
		pthread_create(&thid[i], NULL, thmain, i);
	}
	for (int i = 0; i < NTH; i++)
	{
		pthread_join(thid[i], NULL);
	}

	return EXIT_SUCCESS;
}
