#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid;

    pid = fork();
    if (pid == 0) {
        printf("PID = 0\n");
    } else {
        printf("PID == %d\n", pid);
    }
    getchar();
}
