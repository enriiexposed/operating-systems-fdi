#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
void copy(int fdo, int fdd)
{
    char a[512];
    ssize_t r, w;
    while ((r = read(fdo, a, 512)) > 0)
    {
        w = write(fdd, a, r);
    }
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error: Número de argumentos inválido");
        return -1;
    }
    int fdo = open(argv[1], O_RDONLY);
    if (fdo == -1)
    {
        printf("Error al abrir el archivo de origen");
        close(fdo);
    }
    struct stat sb;
    char *buf;
    if (lstat(argv[1], &sb) == -1)
    {
        perror("Error al llamar a lstat");
        exit(EXIT_FAILURE);
    }
    switch (sb.st_mode & __S_IFMT)
    {
    case __S_IFREG:
        int fdd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0660);
        if (fdd == -1)
        {
            printf("Error al abrir el archivo de destino");
            close(fdd);
        }
        copy(fdo, fdd);
        close(fdd);
        break;
    case __S_IFLNK:
        int nr_bytes = sb.st_size;
        buf = malloc(nr_bytes + 1);
        readlink(argv[1], buf, nr_bytes);
        buf[nr_bytes] = '\0';
        if (symlink(buf, argv[2]) == -1)
        {
            perror("Error al crear el enlace simbolico");
            exit(EXIT_FAILURE);
        }
        break;
    default:
        perror("Tipo de fichero no válido");
        exit(EXIT_FAILURE);
        break;
    }
    close(fdo);
    return 0;
}