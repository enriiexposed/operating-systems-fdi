#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>

struct options {
	char *progname;
	int recurse;
};

struct options opt;

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
        return -1;
    }
    while ((next_dir = readdir(dir)) != NULL) {
        printf("%s\n", next_dir->d_name);
    }
    closedir(dir);
    return 0;
}

/* apartado c */
void process_recurse(char *dirname, char *name)
{
	pid_t pid;
	char path[PATH_MAX];


}

/* apartado c */
void list_dir_recurse(char *name)
{
	DIR *d;
	struct dirent *de;

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


	/********************************************/

	if (opt.recurse)
		printf("%s:\n", dirname);

	list_dir(dirname);

	if (opt.recurse)
		list_dir_recurse(dirname);

	return 0;
}
