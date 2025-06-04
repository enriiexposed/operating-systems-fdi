#include <stdio.h>

int a = 3;

void main() {
    int b=2;
    int p;
    for (int i=0;i<4;i++) {
        p=fork();
        if (p==0) {
            b++;
            execlp("command:..."); // Esta sentencia exec daria error
            a++;
        }
        else {
            wait(NULL);
            a++;
            b--;
        }
    }
    printf("a= %d,b= %d\n",a,b);
}