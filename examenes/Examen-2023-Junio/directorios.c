#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bits/getopt_core.h>

void get_entries_of_dir(char* dirname) {
    DIR *dir;
    struct dirent *next_dir;
    struct stat sb;
    dir = opendir(dirname);

    if (!dir)
    {
        perror("No se ha podido abrir el archivo");
        return -1;
    }

    printf("%s:\n", dirname);
    while ((next_dir = readdir(dir)) != NULL) {
        printf("%s\n", next_dir->d_name);
    }

    while ((next_dir = readdir(dir)) != NULL) {
        if (sb.st_mode & __S_IFMT == __S_IFDIR) {
            if (!strcmp(".", next_dir->d_name) && !strcmp("..", next_dir->d_name)) {
                pid_t pid = fork();
                if (pid == 0) {
                    get_entries_of_dir(next_dir->d_name);
                }
                while (wait(NULL) != -1);
            }
        }
    }
}


int main(int argc, char** argv) {
    char op;
    char Renable = 0;
    while ((op = getopt(argc, argv, "R")) != -1) {
        switch (op) {
            case 'R':
                Renable = 1;
                break;
            default:
                perror("No se ha reconocido ninguna opcion (no hace nada)\n");
            }
    }

    if (Renable) {
        get_entries_of_dir(".");
    }

    return 0;
}