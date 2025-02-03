#include "kernel/param.h"
#include "kernel/types.h"
#include "user.h"
#include "kernel/syscall.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];
  printf("argc = %d, argv[1] = %s\n", argc, argv[1]);
  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }

  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
  

  // 需要接着运行trace后面的程序,否则无法继承到trace()系统调用设置的mask;
  // reference https://gitlab.eduxiji.net/pku2100013145/expProject266346-203747/-/blob/lab2/user/trace.c
  int pid = fork();
  if (pid == 0){
    printf("child(pid=%d): execute with the same systemcallMask of the parent\n", getpid());
    for(i = 2; i < argc && i < MAXARG; i++){
      nargv[i-2] = argv[i];
    }
    exec(nargv[0], nargv);
    exit(0);
  } else if (pid > 0) {
    pid = wait((int *) 0);
    printf("child %d is done\n", pid);
    exit(0);
  }
  exit(0);
}

