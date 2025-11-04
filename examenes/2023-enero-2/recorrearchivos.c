#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int printFichero(char* archivo, int nbytes, char* outfile) {
    char *buf = malloc(sizeof(char) * (nbytes + 1));
    FILE* file = fopen(archivo, "r");
    int len = fread(buf, 1, nbytes, file);

    buf[len] = '\0';

    if (outfile != NULL) {
        FILE* fileout = fopen(outfile, "w+");
        fprintf(fileout, "%s\n", buf);
        fclose(outfile);
    } else {
        printf("%s\n", buf);
    }

    free(buf);
    fclose(file);

    return 0;
}

int leeDir(char *dirname, int nbytes, char* outfile) {
    DIR* dir;
    struct dirent* next_dir;

    dir = opendir(dirname);

    if (!dir) {
        printf("Error al abrir el directorio!\n");
        return -1;
    }


    while ((next_dir = readdir(dir)) != NULL) {
        if(next_dir->d_type == DT_REG && strcmp(next_dir->d_name, ".") != 0 && strcmp(next_dir->d_name, "..") != 0) {
            char filename[255];
            snprintf(filename, 255, "%s/%s", dirname, next_dir->d_name);
            printFichero(next_dir->d_name, nbytes, outfile);
        }
    }

    return 0;
}


int main(int argc, char **argv) {
    int blocks = -1;
    char* outfile = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "n:o:")) != -1) {
        switch(opt) {
            case 'n':
                blocks = strtol(optarg, NULL, 10);
                break;
            case 'o':
                outfile = malloc(sizeof(char) * 255);
                snprintf(outfile, sizeof(outfile), "%s", optarg);
            break;
        }
    }

    if (blocks < 0) {
        printf("Se deben especificar un numero mayor a 0 de bytes a mostrar\n");
    } else {
        leeDir(".", blocks, outfile);
    }
    
    if (outfile != NULL) {
        free(outfile);
    }
    return 0;
}