#include <stdio.h>
#include <string.h>

int main()
{
	char msg[10]; /* array of 10 chars */
	char *p;      /* pointer to a char */
	char msg2[28] = "Hello";  /* msg2 = 'H' 'e' 'l' 'l' 'o' '\0' */

	p   = "Bonjour";
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p = msg2;
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p[0] = 'H', p[1] = 'i', p[2] = '\0';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("msg len: %lu p len %lu msg2 len %lu\n", strlen(msg), strlen(p), strlen(msg2));
	printf("msg size: %lu p size %lu msg2 size %lu\n", sizeof(msg), sizeof(p), sizeof(msg2));

	msg[0] = 'B', msg[1] = 'y';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
}

