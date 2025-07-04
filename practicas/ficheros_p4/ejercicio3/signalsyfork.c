#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bits/getopt_core.h>
#include <signal.h>
#include <errno.h>


/*
En este ejercicio vamos a experimentar el envío de señales, haciendo que un proceso cree a un hijo, 
espere a una señal de un temporizador y, cuando la reciba, termine con la ejecución del hijo.

El programa principal recibirá como argumento el comando del programa que se desea que ejecute el proceso hijo. 
Si a su vez este comando consta de varios argumentos, estos se pasarán separados por espacios a continuación del nombre del programa a ejecutar.

El proceso padre creará un hijo, que cambiará su ejecutable con una llamada a execvp. 
A continuación, el padre establecerá que el manejador de la señal SIGALRM sea una función que envíe una señal SIGKILL al proceso hijo 
y programará una alarma para que le envíe una señal a los 5 segundos. Antes de finalizar, el padre esperará a que finalice el hijo 
y comprobará la causa por la que ha finalizado el hijo (finalización normal o por recepción de una señal), imprimiendo un mensaje por pantalla.
*/

/*
Programa que temporiza la ejecución de un proceso hijo
*/

pid_t pid;

void* handler(int sig) {
	printf("Señal SIGALRM\n");
	fflush(stdout);
	kill(pid, SIGKILL);
}


pid_t launch_command(char **argv)
{
	pid = fork();
	if (pid < 0)
	{
		perror("Error fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		execvp(argv[0], argv);
		perror("Error execvp");
		exit(EXIT_FAILURE);
	}
	return pid;
}

void free_argv(char **cmd_argv, int argc)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		free(cmd_argv[i]);
	}
	free(cmd_argv);
}

char **parse_command(const char *cmd, int *argc)
{
	size_t argv_size = 10;
	const char *end;
	size_t arg_len;
	int arg_count = 0;
	const char *start = cmd;
	char **argv = malloc(argv_size * sizeof(char *));
	if (argv == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (*start && isspace(*start))
		start++;
	while (*start)
	{

		if (arg_count >= argv_size - 1)
		{
			argv_size *= 2;
			argv = realloc(argv, argv_size * sizeof(char *));
			if (argv == NULL)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
		end = start;
		while (*end && !isspace(*end))
			end++;
		arg_len = end - start;
		argv[arg_count] = malloc(arg_len + 1);
		if (argv[arg_count] == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		strncpy(argv[arg_count], start, arg_len);
		argv[arg_count][arg_len] = '\0';
		arg_count++;
		start = end;
		while (*start && isspace(*start))
			start++;
	}
	argv[arg_count] = NULL;
	(*argc) = arg_count;
	return argv;
}

int main(int argc, char **argv)
{
	char** cmd_argv;
	int cmd_argc;
	int i, status;
	if (argc < 2) {
		perror("Nargs insuficiente\n");
		exit(EXIT_FAILURE);
	}

	cmd_argv = parse_command(argv[1], &cmd_argc);

	pid = launch_command(cmd_argv);

	struct sigaction sa;
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = (void*) handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) == -1) {
		printf("No se ha podido configurar la alarma");
		exit(EXIT_FAILURE);
	}
	alarm(5);
	if (waitpid(pid, &status, 0) == -1) {
		perror("Error waitpid");
		exit(EXIT_FAILURE);
	}

	if (WIFEXITED(status)) {
		printf("El proceso hijo finalizó normalmente (en menos de 5 segundos).\n");
	} else if (WIFSIGNALED(status)) {
		printf("El proceso hijo fue terminado por una señal (%d - %s).\n",
			WTERMSIG(status), strsignal(WTERMSIG(status)));
	}
	free_argv(cmd_argv, cmd_argc);
	return 0;
}
