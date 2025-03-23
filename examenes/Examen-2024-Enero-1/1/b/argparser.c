#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "unistd.h"
#include "sys/wait.h"
int setargs(char* args, char** argv) {
    int ret = 0;
    char* it = args;
    char* begin_str = args;

    while(!(*it == '\0')) {
        if (isspace(*it)) {
            if (argv != NULL) {
                *it = '\0';
                it++;
                argv[ret++] = begin_str;
                begin_str = it;
            } else {
                it++; ret++;
            }
        } else {
            it++;
        } 
    }
    if (argv != NULL) {
        argv[ret] = begin_str;
    }
    return ret + 1;
}   

int main(int argc, char** argv) {
    if (argc != 2) {
        perror("El numero de argumentos no es el adecuado\n");
    }

    FILE* f = fopen(argv[1], "r+");

    char linea[256];
    while(fgets(linea, sizeof(linea), f) != NULL) {
        int len = strlen(linea);
        if (linea[len - 1] == '\n') linea[len - 1] = '\0';
        int ntokens = setargs(linea, NULL);
        char** cargv = malloc(sizeof(char*) * (ntokens));
        cargv[ntokens] = NULL;
        
        setargs(linea, cargv);

        printf("Ejecutando comando: ");
        for (int i = 0; i < ntokens; i++) {
            printf("%s ", cargv[i]);
        }
        printf("\n");

        pid_t id = fork();
        
        if (id == -1) {
            perror("Error al crear el nuevo proceso\n");
        } else if (id == 0) {
            execvp(cargv[0], cargv);
            perror("Error en el execvp\n");
            exit(1);
        }

        free(cargv);
    }

    fclose(f);
    return 0;
}