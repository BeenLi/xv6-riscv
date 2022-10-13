#include "kernel/types.h"
#include "user.h"

int main(int argc,char** argv) {
    // 在original process 中, fork 返回子进程的pid
    // 在new process 中, fork 返回0
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0){
        char * pipe_data = malloc(1);
        char * pong = "B";
        read(p[0], pipe_data, 1);
        printf("Child(%d): recieve ping\n", getpid());
        write(p[1], pong, 1); 
        close(p[0]);
        close(p[1]);
        exit(0);
    }
    else if (pid > 0){
        char * pipe_data = malloc(1);
        char * ping="A";
        write(p[1], ping, 1); 
        read(p[0], pipe_data, 1);
        printf("Parent(%d): recieve pong\n", getpid());
        close(p[0]);
        close(p[1]);
        exit(0);
    }
    else{
        printf("fork error \n");
    }
    exit(0);
}