#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main(void)
{
    int fd1, fd2, i, pos;
    char c;
    char buffer[6];
    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    write(fd1, "00000", 5);
    for (i = 1; i < 10; i++)
    {
        pos = lseek(fd1, 0, SEEK_CUR);
        if (fork() == 0)
        {
            /* Child */
            sprintf(buffer, "%d", i * 11111);
            lseek(fd1, pos, SEEK_SET);
            write(fd1, buffer, 5);
            close(fd1);
            exit(0);
        }
        else
        {
            /* Parent */
            lseek(fd1, 5, SEEK_CUR);
            while (wait(NULL) != -1)
                ;
            if (i < 9)
                write(fd1, "00000", 5);
        }
    }
    lseek(fd1, 0, SEEK_SET);
    printf("File contents are:\n");
    while (read(fd1, &c, 1) > 0)
        printf("%c", (char)c);
    printf("\n");
    close(fd1);
    exit(0);
}