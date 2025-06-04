// programB
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER_SIZE 512

int main( void ) {
    int fd;
    char c;
    symlink("file.txt","anotherfile");
    fd=open("anotherfile", O_RDWR);
    lseek(fd, 16*BUFFER_SIZE, SEEK_END);
    write(fd,"z",1);
    lseek(fd, 0, SEEK_SET );
    read(fd, &c, 1);
    printf(" %c\n",c);
    return 0;
}