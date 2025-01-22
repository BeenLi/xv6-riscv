#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user.h"

int main(int argc,char** argv) {
    // 在original process 中, fork 返回子进程的pid
    // 在new process 中, fork 返回0
    int p[2];
    /* 2025/1/22
    // 如果只使用一个pipe，还要实现双向通信，那么只能传输一次请求：
        1.
    子读，关闭读(如果一直读就会出错,因为会无限期等待自己的写)，子写，关闭写
        2. 父写，父关闭写，父读，关闭读
    // 要么使用两个pipe,分别用于父写子读，子写父读；
        1. 子读pipe0, 关闭pipe0写;写pipe1,关闭pipe1读
        2. 父写pipe0，关闭pipe0读；读pipe1,关闭pipe1写
    */
    pipe(p);
    int pid = fork();
    if (pid == 0){
      char* pipe_data = malloc(5);
      char  pong[5]   = "Pong\0";
      /*
      int   a         = read(p[0], pipe_data, 5);
      while (a != 0) {
        printf(
            "Child(%d): recieve %s(word count:%d)\n", getpid(), pipe_data, a);
        a = read(
            p[0], pipe_data,
            5); // 如果不关闭p[1], 那么一直在等待自己的p[1]会造成死锁(得沟通好);
        printf("Child(%d): read another %d words\n", getpid(), a);
      }
      printf("Child(%d): I get out of the while loop\n", getpid());
      */

      int a = read(p[0], pipe_data, 5);
      printf("Child(%d): recieve %s(word count:%d)\n", getpid(), pipe_data, a);
      close(p[0]);
      // 管道是匿名的，一旦关闭就不能重新打开；除非
      // open(p[1], O_RDWR);
      write(p[1], pong, 5);
      close(p[1]);
      exit(0);
    }
    else if (pid > 0){
      char* pipe_data = malloc(5);
      char  ping[5]   = "Ping\0";
      write(p[1], ping, 5);
      close(p[1]);
      int a = read(p[0], pipe_data, 5);
      printf("Parent(%d): recieve %s(word count:%d)\n", getpid(), pipe_data, a);
      while (a != 0) {
        a = read(p[0], pipe_data, 5);
        printf("Parent(%d): read another %d words\n", getpid(), a);
      }
      close(p[0]);
      exit(0);
    }
    else{
        printf("fork error \n");
    }
    exit(0);
}