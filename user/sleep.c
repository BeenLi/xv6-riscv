#include "kernel/types.h"
#include "user.h"

int parse_int(const char* arg) {
    const char* p = arg;
    for ( ; *p ; p++ ) {
        if ( *p < '0' || *p > '9' ) {
            return -1;
        }
    }
    return atoi(arg);
}

int main(int argc,char** argv) {
    int time;
    if (argc != 2) {
        printf("you must input one argument only\n");
        exit(0);
    } 
    
    time = parse_int(argv[1]);
    if (time < 0) {
        printf("error argument : %s\n",argv[1]);
        exit(0);
    }
    sleep(time);

    /*  -- test fork --
    // TODO:两个线程会同时printf, 导致printf混乱
    int parent_variable = 100;
    int pid             = fork();
    if (pid > 0) {
        printf("parent, child's pid = %d\n", pid);
        printf(
            "parent, parent_variable(addr:%#x, value:%d)\n", &parent_variable,
            parent_variable);
        parent_variable = 200;
        printf(
            "parent, change parent_variable from 100 to %d\n", parent_variable);
        // way1: Point to an already-existing integer
        // int status = 0;
        // int* child_exit_status = &status;

        // way2: Dynamically allocate memory for the pointer
        int* child_exit_status = malloc(sizeof(int));
        if (!child_exit_status) {
            printf("parent: malloc failed");
            return 1;
        }
        // wait for a child to exit; exit status in
        // *status; return child PID;
        pid = wait((int*)child_exit_status);
        printf(
            "parent: child %d is done with exit status of %d\n", pid,
            *child_exit_status);
        printf(
            "parent, parent_variable(addr:%p, value:%d)\n", &parent_variable,
            parent_variable);
        // way2: need release the dynamically allocated memory
        free(child_exit_status);
    } else if (pid == 0) {
        printf(
            "child, parent_variable(addr:%#x, value:%d)\n", &parent_variable,
            parent_variable);
        parent_variable = 400;
        printf(
            "child, change parent_variable from 100 to %d\n", parent_variable);
        printf("child: will exit\n");
        exit(0);
    } else {
        printf("fork error\n");
    }
    */
    exit(0);
}