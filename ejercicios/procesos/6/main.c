#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>

int fd = -1;
char buf1[4]="aaaa";
char buf2[4]="bbbb";

void* thread1(void* arg) {
    fd=open("test",O_RDWR|O_CREAT|O_TRUNC,0666);
    write(fd,buf1,4);
}

void* thread2(void* arg) {
    while (fd==-1) {};
    write(fd,buf2,4);
}

int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1,NULL,thread1,NULL);
    pthread_create(&tid2,NULL,thread2,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    close(fd);
}
