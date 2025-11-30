#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
/** Loads a string from a file.
*
* file: pointer to the FILE descriptor
*
* The loadstr() function must allocate memory from the heap to store
* the contents of the string read from the FILE.
* Once the string has been properly built in memory, the function
returns
* the starting address of the string (pointer returned by malloc())
*
* Returns: !=NULL if success, NULL if error
*/
char *loadstr(FILE *file) {
	int length = 0;
	int init = ftell(file);
	int c;
	while ((c = fgetc(file)) != EOF && c != '\0') {
		length++;
	}

	if (c == EOF) {
		return NULL;
	}

	fseek(file, init, SEEK_SET);
	char *str = (char *) malloc((length + 1) * sizeof(char));
	fread(str, sizeof(char), length + 1, file);
	return str;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 2;
	}

	FILE *file = fopen(argv[1], "rb");
	if (file == NULL) {
		fprintf(stderr, "The file could not be opened: ");
		perror(argv[1]);
		return 1;
	}

	char *str;
	while ((str = loadstr(file)) != NULL) {
		printf("%s\n", str);
		free(str);
	}

	fclose(file);
	return 0;
}