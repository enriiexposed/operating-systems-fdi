#include "stdio.h"
#include "string.h"
#include "ctype.h"
int setargs(char* args, char** argv) {
    int ret = 0;
    char* it = args;
    char* begin_str = args;

    // Quitar espacios al principio
    while(issspace(*it++));

    while(*it != '\0') {
        if (isspace(*it)) {
            if (argv != NULL) {
                *it = '\0';
                it++;
                argv[ret] = begin_str;
                begin_str = it;
            }
            ret++;
        } else {
            it++;
        } 
    }

    return *(it - 1) != ' ' ? ret + 1 : ret;
}   

int main(int argc, char** argv) {
    if (argc != 2) {
        perror("El numero de argumentos no es el adecuado\n");
    }

    char args_parsed[6][50];
    printf("Number of args: %d\n", setargs(argv[1], args_parsed));
    return 0;
}