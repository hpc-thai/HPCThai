#include <stdio.h>
#include <unistd.h>

int balance = 0;

int main()
{
    int pid;

    pid = fork();
    if (pid == 0) {
        int i;
        printf("PID = 0\n");
        for (i=0; i<100000; i++) balance += 3;
    } else {
        int i;
        printf("PID == %d\n", pid);
        for (i=0; i<100000; i++) balance += 7;
    }
    getchar();
    printf("balance = %d\n", balance);
}
