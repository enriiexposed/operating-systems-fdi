#include <stdio.h>
#include <string.h>

void copy2(char* org, char** dst)
{
	*dst = org;
}

void copy(char* org, char* dst)
{
	dst = org;
}

void mod(char* org, char* dst)
{
	int i;

	for (i=0;i<strlen(org);i++)
		dst[i] = org[i] - 32;
}

int main()
{
	char* str1 = "original";
	char* str2 = "other";
	char str3[32];

	copy(str1, str2);
	//copy2(str1, &str2);
	printf("str1 %s str2 %s\n", str1, str2);

	mod(str1, str3);
	printf("str1 %s str3 %s\n", str1, str3);

	return 0;
}

