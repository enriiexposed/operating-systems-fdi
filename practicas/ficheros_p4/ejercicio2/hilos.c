#include <stdlib.h>
#include <stdio.h>

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define NTH 10
struct tharg
{
	int i;
	char p;
};

void *thread_usuario(void *arg)
{
	struct tharg *args = (struct tharg*) arg;
	printf("Thread %d with thread id %d has priority %c", args->i, pthread_self(), args->p);
}

int main(int argc, char *argv[])
{
	int j = 0;
	pthread_attr_t attr;
	pthread_t thid[NTH];

	pthread_attr_init(&attr);
	for(j = 0; j<NTH; j++) {
		struct tharg* args = malloc(sizeof(struct tharg));
		if (args == NULL) {
			return -1;
		}
		args->i = j;
		args->p = (j % 2 == 0) ? 'P': 'N';
		if (pthread_create(&thid[j], &attr, thread_usuario, args)) {
			free(args);
			return -1;
		}
		free(args);
	}

	for (int i = 0; i<NTH; i++) {
		pthread_join(thid[j], NULL);
	}
	return 0;
}
