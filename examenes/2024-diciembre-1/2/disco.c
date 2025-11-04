#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define CAPACITY 3
#define VIPSTR(vip) ((vip) ? " vip " : "not vip")
struct tharg
{
	int id;
	int vip;
	int dancefloor
};
int turn = 0, turn_vip = 0;
int ticket = 0, ticket_vip = 0;
int nclients = 0, vip_waiting = 0;
sem_t pista1, pista2;
pthread_mutex_t m;
pthread_cond_t queue, queue_vip, dancefloor1, dancefloor2;
void enter_normal_client(int id)
{
	int my_turn;
	pthread_mutex_lock(&m);
	my_turn = ticket++;
	while ((turn != my_turn) || (vip_waiting > 0) || (nclients >= CAPACITY))
	{
		pthread_cond_wait(&queue, &m);
	}
	nclients++;
	turn++;
	printf("Client %2d (not vip) enter disco\n", id);
	if (vip_waiting > 0)
		pthread_cond_broadcast(&queue_vip);
	else
		pthread_cond_broadcast(&queue);
	pthread_mutex_unlock(&m);
}
void enter_vip_client(int id)
{
	int my_turn;
	pthread_mutex_lock(&m);
	my_turn = ticket_vip++;
	vip_waiting++;
	while ((turn_vip != my_turn) || (nclients >= CAPACITY))
	{
		pthread_cond_wait(&queue_vip, &m);
	}
	vip_waiting--;
	nclients++;
	turn_vip++;
	printf("Client %2d (vip) enter disco\n", id);
	if (vip_waiting > 0)
		pthread_cond_broadcast(&queue_vip);
	else
		pthread_cond_broadcast(&queue);
	pthread_mutex_unlock(&m);
}
/**
 * Esta solucion sirve porque entiendo que si hay dos clietnes dentro, se debe esperar (eso es lo que dice el enunciado)
 */
void dance(int id, int isvip, int dancefloor)
{
	if (dancefloor == 1) {
		sem_wait(&pista1);
	} else {
		sem_wait(&pista2);
	}	
	printf("Client %2d (%s) dancing on dancefloor %d\n", id, VIPSTR(isvip), dancefloor);
	sleep((rand() % 3) + 1);
}
void disco_exit(int id, int isvip, int dancefloor)
{
	pthread_mutex_lock(&m);
	nclients--;
	printf("Client %2d (%s) exit disco\n", id, VIPSTR(isvip));
	if (vip_waiting > 0)
		pthread_cond_broadcast(&queue_vip);
	else
		pthread_cond_broadcast(&queue);
	pthread_mutex_unlock(&m);
	if (dancefloor == 1) {
		sem_post(&pista1);
	} else {
		sem_post(&pista2);
	}
}
void *client(void *arg)
{
	struct tharg *tharg = (struct tharg *)arg;
	if (tharg->vip == 1)
	{
		enter_vip_client(tharg->id);
	}
	else
	{
		enter_normal_client(tharg->id);
	}
	dance(tharg->id, tharg->vip, tharg->dancefloor);
	disco_exit(tharg->id, tharg->vip, tharg->dancefloor);
	free(tharg);
}
int main(int argc, char *argv[])
{
	int j, m;
	pthread_attr_t attr;
	char *filename;
	int vip;
	int dancefloor;
	if (argc != 2)
	{
		perror("Número de argumentos inválido");
		exit(EXIT_FAILURE);
	}
	filename = argv[1];
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	fscanf(file, "%d ", &m);
	sem_init(&pista1, 0, 2);
	sem_init(&pista2, 0, 2);
	pthread_t *thid = malloc(sizeof(pthread_t) * m);
	pthread_attr_init(&attr);
	for (j = 0; j < m; j++)
	{
		struct tharg *args = malloc(sizeof(struct tharg));
		if (args == NULL)
		{
			perror("Error malloc");
			fclose(file);
			exit(EXIT_FAILURE);
		}
		args->id = j;
		fscanf(file, "%d %d", &vip, &dancefloor);
		args->vip = (vip == 0) ? 0 : 1;
		args->dancefloor = dancefloor;
		if (pthread_create(&thid[j], &attr, client, args) != 0)
		{
			perror("Error creating thread");
			free(args);
			fclose(file);
			exit(EXIT_FAILURE);
		}
	}
	fclose(file);
	for (j = 0; j < m; j++)
	{
		pthread_join(thid[j], NULL);
	}
	sem_close(&pista1);
	sem_close(&pista2);
	return 0;
}