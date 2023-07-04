#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 100000

void foo() {
  int x = 0;
  x = x + 10;
}


int main() {
   time_t start, end;
   double diff;
   pid_t pid[N];
   int status[N], i;

   start = time(NULL);
   printf("Arxiki timi deuteroleptwn: %d s\n", (int) start);

   i = 0;
   while (i< N) {
      pid[i] = fork();

      if (pid[i] == 0) {
        foo();
        exit(0);
      }
      i++;
   }

   i = 0;
   while (i<N) {
     waitpid(pid[i], &status[i], 0);
     i++;
   }

   end = time(NULL);
   diff = difftime(end, start);
   printf("Teliki timi deuteroleptwn: %d s\n", (int) end);
   printf("end - start: %d s\n", (int) diff);
   printf("MO: %f s\n", diff/N);

   return(0);
}
