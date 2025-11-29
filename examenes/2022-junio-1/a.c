#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

struct args {
    int id;
    int isvip;
};

pthread_t threads[N];

void dance() {
    sleep((rand() % 3) + 1);
}

void* client(void* arg) {
    struct args* a = (struct args*)arg;
    printf("Client %d, VIP status: %d\n", a->id, a->isvip);
    dance();
}

int main()  {
    for (int i = 0; i < N; i++) {
        struct args* a = malloc(sizeof(struct args));
        a->id = i + 1;
        a->isvip = (rand() % 2); // Randomly assign VIP status
        pthread_create(&threads[i], NULL, client, a);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
}