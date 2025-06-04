#include <stdio.h>
#include <sys/types.h>
#define N 8

void exec_childs(int arr[], int n);

int main() {
    pid_t pids[N + 1];

    for(int i = 0; i < ; i++) {
        char buf[10];
        snprintf(buf, sizeof(buf), "./p%d", arr[i]);
        pid_t pid = fork();
        if (pid == 0) {
            execl(buf, buf, (char*) NULL);
            perror("error\n");
        }
    }
}

void exec_childs(int arr[], int n) {
    
}