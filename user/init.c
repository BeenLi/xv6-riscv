// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
      printf("init: starting sh(mypid=%d)\n", getpid());
      pid = fork();
      if (pid < 0) {
          printf("init: fork failed\n");
          exit(1);
      }
      if (pid == 0) {
          exec("sh", argv);
          printf("init: exec sh failed\n");
          exit(1);
      }

      for (;;) {
          // this call to wait() returns if the shell exits,
          // or if a parentless process exits(orphaned process).

          // tips: 2025/1/22
          // In a traditional Unix-like system (including xv6), if a process’s
          // parent dies before the child does, that child is reparented to
          // PID 1—in xv6, that’s the init process. Essentially, orphaned
          // processes “inherit” init as their new parent. When such a process
          // finally exits, its wait() status goes to init.
          wpid = wait((int*)0);
          if (wpid == pid) {
              // the shell exited; restart it.
              break;
          } else if (wpid < 0) {
              printf("init: wait returned an error\n");
              exit(1);
          } else {
              // it was a parentless process; do nothing.
          }
      }
  }
}
