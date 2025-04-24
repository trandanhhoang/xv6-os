#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Write a program that uses UNIX system calls to ''ping-pong'' a byte
// between two processes over a pair of pipes, one for each direction. 

int
main(int argc, char *argv[])
{

  int fds[2];
  // char buf[100];
  // int n;

  if(argc != 1){
    fprintf(2, "Usage: pingpong\n");
    exit(0);
  }

  // create a pipe, with two FDs in fds[0], fds[1].
  pipe(fds);

  // The parent should send a byte to the child; the child should print "<pid>:
// received ping", where <pid> is its process ID, write the byte on the pipe 
// to the parent, and exit; the parent should read the byte from the child, 
// print "<pid>: received pong", and exit. Your solution should be in the file user/pingpong.c.

  char buf;

  if(fork() == 0){
    // this is child
    read(fds[0], &buf, 1);
    close(fds[0]);

    fprintf(1, "%d: received ping\n", getpid());

    write(fds[1], &buf, 1);
    close(fds[1]);
  
    exit(0);
  }else{
    // this is parent
    write(fds[1], &buf, 1);
    close(fds[1]);

    read(fds[0], &buf, 1);
    fprintf(1, "%d: received pong\n", getpid());
    close(fds[0]);
    exit(0);
  }
  
  exit(0);
}
