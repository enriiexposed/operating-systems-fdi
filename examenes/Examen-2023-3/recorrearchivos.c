#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int printFichero(char* archivo, int nbytes) {
    int fd, bytes_read;
    char *buf;
    fd = open(archivo, O_RDONLY , 0666);

    if (fd == -1) {
        perror("No se pudo abrir el fichero\n");
        return -1;
    }

    buf =  malloc(sizeof(char) * (nbytes + 1));
    bytes_read = read(fd, buf, nbytes);
    buf[nbytes] = '\0';
    printf("%s\n", archivo);
    printf("Primeros %d bytes:\n%s\n", nbytes, buf);

    free(buf);
    close(fd);
    return 0;
}

int leeDir(char *namedir, int nbytes) {
    DIR* dir;
    struct dirent* next_dir;
    struct stat sb;

    dir = opendir(namedir);

    if (!dir) {
        printf("Error!\n");
        return -1;
    }

    while((next_dir = readdir(dir)) != NULL) {
        if (strcmp(next_dir -> d_name, ".") != 0 && strcmp(next_dir->d_name, "..") != 0) {
            char archivo[256];
			snprintf(archivo, sizeof(archivo), "%s/%s", namedir, next_dir->d_name);
            if (lstat(archivo, &sb) == -1) {
                perror("Error al hacer lstat!\n");
                return -1;
            }
            switch(sb.st_mode & __S_IFMT) {
                case __S_IFREG:
                    if (printFichero(next_dir->d_name, nbytes)) {
                        perror("No se pudo leer el fichero\n");
                        return -1;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    closedir(namedir);

    return 0;
}


int main(int argc, char **argv) {
    int blocks = 0;
    char* outfile = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "n:o:")) != -1) {
        switch(opt) {
            case 'n':
            blocks = strtol(optarg, NULL, 10);
            break;
            case 'o':
            outfile = optarg;
            break;
        }
    }

    leeDir(".", blocks);
    return 0;
}