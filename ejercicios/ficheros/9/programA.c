// programA
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER_SIZE 512

char buf[BUFFER_SIZE];
static void init_buf(void) {
    for (int i = 0; i < BUFFER_SIZE; ++i) buf[i] = 'y';
}

int main(void) {
    int fd;
    init_buf();
    fd = open( "file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644 );
    write(fd,"x",1);
    lseek(fd, 8*BUFFER_SIZE, SEEK_CUR);
    write(fd,buf,BUFFER_SIZE);
    return 0;
}