#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>



void  *reader(void*);
void  *writer(void*);
sem_t wsem;
pthread_mutex_t mutex;
int readcount = 0;
int counter = 1;


int main()
{
   pthread_t r[9], w[4];
   int a[9] = {1,2,3,4,5,6,7,8,9};	
   system("clear");
   sem_init(&wsem,0,1);
   pthread_mutex_init(&mutex, NULL);
   for (int i=0; i<9; i++) {
     pthread_create(&r[i],NULL,reader,(void*)&a[i]);
   }
   for (int i=0; i<4; i++) {
     pthread_create(&w[i],NULL,writer,(void*)&a[i]);
   }

   for (int i=0; i<9; i++) {
     pthread_join(r[i],NULL);
   }
   for (int i=0; i<4; i++) {
     pthread_join(w[i],NULL);
   }
  
   pthread_mutex_destroy(&mutex);
   sem_destroy(&wsem);

   return(0);
}

void *reader(void *rno)
{
  printf("\nReader %d is created", (*((int *)rno)));
  sleep(1);
  pthread_mutex_lock(&mutex);
  readcount++;
  /* An prokeitai gia ton prwto reader, o writer blockaretai */
  if(readcount==1) {
    sem_wait(&wsem);
  }
  pthread_mutex_unlock(&mutex);
  printf("\nReader %d is reading\n", (*((int *)rno)));
  sleep(1);
  printf("\nReader %d finished reading, counter value: %d\n", (*((int *)rno)), counter);
  pthread_mutex_lock(&mutex);
  readcount--;
  /* An prokeitai gia ton teleutaio reader, ksupnaei o writer */
  if(readcount==0) {
   sem_post(&wsem);
  }
  pthread_mutex_unlock(&mutex);
}

 void* writer(void *wno)
{
  printf("\nWriter %d is created", (*((int *)wno)));
  sleep(1);
  sem_wait(&wsem);
  printf("\nWriter %d is writing\n", (*((int *)wno)));
  counter = 2 * counter;
  sleep(1);
  printf("\nWriter %d finished writing, counter value: %d\n", (*((int *)wno)), counter);
  sem_post(&wsem);
}  
