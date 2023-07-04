#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

int main() {
   int i;
   int pid;

   for (i=0; i<N; i++) {
      pid = fork();

      if (pid > 0) {
       printf(" Father = %d, Id = %d, Child = %d\n", getppid(),getpid(),pid);
       wait(NULL);
       exit(0);
      }
   }
   return(0);
}
