#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>
#include <bits/getopt_core.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>

struct options {
	char *progname;
	int recurse;
};

struct options opt;

/* Declaraciones forward de funciones */
void process_recurse(char *dirname, char *name);

void usage(void)
{
	printf("%s [options] [dirname]\n\n", opt.progname);
	printf("lists the contents of dirname (default .)\n");
	printf("options:\n"
		"\t-h:\tshow this help\n"
		"\t-R:\trecursive\n"
	);
}

/* apartado b */
void list_dir(char *name)
{
	DIR *dir;
    struct dirent *next_dir;
    dir = opendir(name);
    if (!dir)
    {
        perror("No se ha podido abrir el archivo");
        exit(EXIT_FAILURE);
    }
    while ((next_dir = readdir(dir)) != NULL) {
        printf("%s\n", next_dir->d_name);
    }
    closedir(dir);
}

/* apartado c */
void list_dir_recurse(char *name)
{	
	printf("\n");
	// llamadas recursivas
	DIR *dir;
    struct dirent *next_dir;
	struct stat sb;
    dir = opendir(name);
    if (!dir)
    {
        perror("No se ha podido abrir el archivo");
        return;
    }
    while ((next_dir = readdir(dir)) != NULL) {
        if (strcmp(next_dir->d_name, ".") != 0 && strcmp(next_dir->d_name, "..") != 0)
        {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", name,
                     next_dir->d_name);
            if (lstat(path, &sb) == -1)
            {
                perror("Error al llamar a lstat");
				exit(EXIT_FAILURE);
            }
            switch (sb.st_mode & __S_IFMT)
            {
            case __S_IFDIR:
				printf("%s:\n", path);
                process_recurse(path, name);
                break;
            default:
                break;
            }
        }
    }
    closedir(dir);
}

/* apartado c */
void process_recurse(char *dirname, char *name)
{
	// llamada al contenido que hay en el directorio (recursivo)
	pid_t pid;
	pid = fork();

	if (pid == -1) {
		perror("Error al hacer el fork\n");
		exit(1);
	} else if (pid == 0) {
		// Ejecutar el mismo programa recursivamente con -R y el subdirectorio
		execl("./ej1", "ej1", "-R", dirname, NULL);
		perror("Error al ejecutar el proceso hijo");
		exit(1);
	} else {
		int status;
		waitpid(pid, &status, 0);
	}
}



int main(int argc, char **argv)
{
	char *dirname = ".";
	int o;
	opt.progname = argv[0];
	opt.recurse = 0;

	int opcion;

	/* Apartado a: procesar opciones con getopt */
	/* Parse command-line options */
	while((opcion = getopt(argc, argv, "R")) != -1) {
		switch(opcion) {
		case 'R':
			opt.recurse = 1;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	// Después de procesar las opciones, leer el directorio si se especificó
	if (optind < argc) {
		dirname = argv[optind];
	}

	/********************************************/

	list_dir(dirname);

	if (opt.recurse)
		list_dir_recurse(dirname);

	return 0;
}
