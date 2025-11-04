#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bits/getopt_core.h>

void get_entries_of_dir(char* dirname, char R) {
    DIR *dir;
    struct dirent *next_dir;
    struct stat sb;
    dir = opendir(dirname);

    if (!dir) {
        perror("No se ha podido abrir el archivo");
        return;
    }

    while ((next_dir = readdir(dir)) != NULL) {
        if (strcmp(next_dir->d_name, ".") != 0 && strcmp(next_dir->d_name, "..") != 0) {
            printf("%s\n", next_dir->d_name);
            if (R == 1 && next_dir->d_type == DT_DIR) {
                char path[256];
                snprintf(path, sizeof(path), "%s/%s", dirname, next_dir->d_name);
                pid_t pid = fork();
                if (pid == 0) {
                    get_entries_of_dir(path, R);
                    exit(0);
                }
                while (wait(NULL) != -1);
            }
        }
    }

    closedir(dir);
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

    get_entries_of_dir(".", Renable);     

    return 0;
}