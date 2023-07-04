#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

int main() {
  int pid, i;

  for (i=0; i<N; i++)  {
    pid = fork();
    if (pid == 0) {
      printf("My pid is %d and my parent's id is %d\n", getpid(), getppid());
      exit(0);
    }
  }

  for (i=0; i<N; i++)
    wait(NULL);

  return(0);
}
