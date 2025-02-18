#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char *argv[])
{
	FILE *file = NULL;
	int c, ret;
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}
	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);
	/* Read file byte by byte */
	// while ((c = getc(file)) != EOF) {
	while ((fread(&c, sizeof(c), 1, file)) == 1)
	{
		/* Print byte to stdout */
		// ret=putc((unsigned char) c, stdout);
		ret = fwrite(&c, sizeof(c), 1, stdout);
		if (ret == EOF)
		{
			fclose(file);
			err(3, "putc() failed!!");
		}
	}
	fclose(file);
	return 0;
}