#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <bits/getopt_core.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		printf("Error: Número de argumentos inválido");
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		printf("Error al abrir el archivo");
		return -1;
	}
	int opt;
	int n = 0;
	/* Parse command-line options */
	lseek(fd, 0, SEEK_SET);
	while ((opt = getopt(argc, argv, "n:e")) != -1)
	{
		switch (opt)
		{
		case 'n':
			sscanf(optarg, "%d", &n);
			lseek(fd, n, SEEK_SET);
			break;
		case 'e':
			lseek(fd, -n - 1, SEEK_END);
			break;
		default:
			break;
		}
	}
	char *buf;
	buf = malloc(sizeof(char) * n);
	while (read(fd, buf, sizeof(char)) > 0)
	{
		printf("%s", buf);
	}
	close(fd);
	return 0;
}