#include <stdio.h>
#include <stdlib.h>

int main()
{
   int pid1;
   int pid2;
   pid1=fork();
   if(pid1<0)
     printf("Could not create any child\n");
   else
   {
     pid2=fork();
     if(pid2<0)
       printf("Could not create any child\n");
     else if((pid1<1)&&(pid2<0)) kill(pid1,9);
     else printf("pid: %d, ppid: %d\n",getpid(), getppid());
   }
   sleep(20);
   

   return(0);
}
