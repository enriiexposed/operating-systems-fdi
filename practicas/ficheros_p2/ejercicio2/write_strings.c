#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: <file_name> <strngs> ...\n");
		exit(EXIT_FAILURE);
	}

	FILE *file;
	if ((file = fopen(argv[1], "wb")) == NULL) {
		fprintf(stderr, "The file could not be opened: ");
		perror(NULL);
	}

	for (int i = 2; i < argc; i++) {
		fwrite(argv[i], sizeof(char), strlen(argv[i]) + 1, file);
	}

	fclose(file);
	return 0;
}