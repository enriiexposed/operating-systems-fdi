#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
void copy(int fdo, int fdd)
{
	char a[512];
	ssize_t r, w;
	while ((r = read(fdo, a, 512)) > 0)
	{
		w = write(fdd, a, r);
	}
}
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Error");
		return -1;
	}
	int fdo = open(argv[1], O_RDONLY);
	if (fdo == -1)
	{
		printf("Error al abrir el archivo de origen");
		close(fdo);
	}
	int fdd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0660);
	if (fdd == -1)
	{
		printf("Error al abrir el archivo de destino");
		close(fdd);
	}
	copy(fdo, fdd);
	close(fdo);
	close(fdd);
	return 0;
}