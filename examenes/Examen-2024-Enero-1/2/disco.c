#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define CAPACITY 3
#define VIPSTR(vip) ((vip == 1) ? " vip " : (vip == 0) ? "not vip" : " special ")
struct tharg
{
	int id;
	int vip;
};

int turn = 0, turn_vip = 0, turn_special = 0;
int ticket = 0, ticket_vip = 0, ticket_special = 0;
int nclients = 0, vip_waiting = 0, special_waiting = 0;
int nspecials = 0;

pthread_mutex_t m;
pthread_cond_t queue, queue_vip, queue_specials;

void enter_normal_client(int id) {
	int my_turn;
	pthread_mutex_lock(&m);
	my_turn = ticket++;
	/**
	 * Espera a que sea mi turno, no hayan vips y la sala no este llena
	 */
	while ((turn != my_turn) || (vip_waiting > 0) || (special_waiting > 0) || (nclients >= CAPACITY) || (nclients - nspecials == 0))
	{
		pthread_cond_wait(&queue, &m);
	}
	nclients++;
	turn++;
	printf("Client %2d (not vip) enter disco\n", id);
	dance(id, 0);
	if (special_waiting > 0) {
		pthread_cond_broadcast(&queue_specials);
	} else if (vip_waiting > 0)
		pthread_cond_broadcast(&queue_vip);
	else
		pthread_cond_broadcast(&queue);
	pthread_mutex_unlock(&m);
}

void enter_vip_client(int id) {
	int my_turn;
	pthread_mutex_lock(&m);
	my_turn = ticket_vip++;
	vip_waiting++;
	/**
	 * 	Espera a que sea mi turno y a que no este llena la sala
	 */
	while ((turn_vip != my_turn) || (nclients >= CAPACITY) || (special_waiting > 0) || (nclients - nspecials == 0)) {
		pthread_cond_wait(&queue_vip, &m);
	}
	nspecials++;
	vip_waiting--;
	nclients++;
	turn_vip++;
	printf("Client %2d (vip) enter disco\n", id);
	dance(id, 1);
	if (special_waiting > 0) {
		pthread_cond_broadcast(&queue_specials);
	}
	else if (vip_waiting > 0)
		pthread_cond_broadcast(&queue_vip);
	else
		pthread_cond_broadcast(&queue);
	pthread_mutex_unlock(&m);
}

void enter_special_client(int id) {
	int my_turn;
	pthread_mutex_lock(&m);
	special_waiting++;
	my_turn = ticket_special++;
	while(turn_special != my_turn || (nclients >= CAPACITY) || (nclients - nspecials > 0)) {
		pthread_cond_wait(&queue_specials, &m);
	}
	printf("Client special enters disco\n");
	dance(id, 2);
	special_waiting--;
	nspecials++;
	turn_special++;
	pthread_mutex_unlock(&m);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}
void disco_exit(int id, int isvip)
{
	pthread_mutex_lock(&m);
	if (isvip == 2) nspecials--;
	nclients--;
	printf("Client %2d (%s) exit disco\n", id, VIPSTR(isvip));
	if (special_waiting > 0) {
		pthread_cond_broadcast(&queue_specials);
	} else if (vip_waiting > 0)
		pthread_cond_broadcast(&queue_vip);
	else
		pthread_cond_broadcast(&queue);
	pthread_mutex_unlock(&m);
}
void *client(void *arg)
{
	struct tharg *tharg = (struct tharg *)arg;
	if (tharg->vip == 1)
	{
		enter_vip_client(tharg->id);
	}
	else if (tharg->vip == 0)
	{
		enter_normal_client(tharg->id);
	} else enter_special_client(tharg->id);

	disco_exit(tharg->id, tharg->vip);
	free(tharg);
}
int main(int argc, char *argv[])
{
	int j, m;
	pthread_attr_t attr;
	char *filename;
	int vip;
	if (argc != 2) {
		perror("Número de argumentos inválido");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	fscanf(file, "%d ", &m);
	pthread_t *thid = malloc(sizeof(pthread_t) * m);
	pthread_attr_init(&attr);
	for (j = 0; j < m; j++) {
		struct tharg *args = malloc(sizeof(struct tharg));
		if (args == NULL) {
			perror("Error malloc");
			fclose(file);
			exit(EXIT_FAILURE);
		}

		args->id = j;
		fscanf(file, "%d ", &vip);
		args->vip = (vip == 0) ? 0 : vip == 1 ? 1 : 2;
		if (pthread_create(&thid[j], &attr, client, args) != 0) {
			perror("Error creating thread");
			free(args);
			fclose(file);
			exit(EXIT_FAILURE);
		}
	}

	fclose(file);

	for (j = 0; j < m; j++) {
		pthread_join(thid[j], NULL);
	}
	return 0;
}