#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <zconf.h>
#include <sys/wait.h>

#define ONE 1

typedef sem_t Semaphore;
Semaphore *mutex;

struct Heap{
    int *arr;
    int count;
    int capacity;
};
typedef struct Heap Heap;

Heap *CreateHeap(int capacity){
    Heap *h = (Heap *) malloc(sizeof(Heap));
    if (h == NULL)
        printf("Memory Error!");

    h -> count=0;
    h -> capacity = capacity;
    h -> arr = (int *) malloc(capacity * sizeof(int));

    if (h -> arr == NULL)
        printf("Memory Error!");
    return h;
}

void heapify_bottom_top(Heap *h, int index){
    int temp;
    int parent_node = (index-1)/2;

    if (h -> arr[parent_node] > h -> arr[index]) {
        temp = h -> arr[parent_node];
        h -> arr[parent_node] = h -> arr[index];
        h -> arr[index] = temp;
        heapify_bottom_top(h,parent_node);
    }
}

void insert(Heap *h, int key){
    if (h -> count < h -> capacity) {
        h -> arr[h->count] = key;
        heapify_bottom_top(h, h -> count);
        h -> count++;
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    int left = parent_node*2 + 1;
    int right = parent_node*2 + 2;
    int min;
    int temp;

    if (left >= h->count || left < 0)
        left = -1;
    if (right >= h->count || right < 0)
        right = -1;

    if (left != -1 && h -> arr[left] < h -> arr[parent_node])
        min = left;
    else
        min = parent_node;
    if (right != -1 && h -> arr[right] < h -> arr[min])
        min = right;

    if (min != parent_node){
        temp = h -> arr[min];
        h -> arr[min] = h -> arr[parent_node];
        h -> arr[parent_node] = temp;

        heapify_top_bottom(h, min);
    }
}

void print(Heap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0; i< h -> count; i++) {
        printf("-> %02d ",h->arr[i]);
    }
    printf("->__/\\__\n");
}

int main() {
    int i;
    key_t shmkey;
    int shmid;
    pid_t pid;

    Heap *shared_heap;
    int *rand_priority;

    srand(time(NULL));

    shmkey = ftok("/dev/null", 5);  // Shared variable
    shmid = shmget(shmkey, sizeof(int), 0644 | IPC_CREAT);

    if (shmid < 0) {    // Memory check
        perror("shmget\n");
        exit(1);
    }

    shared_heap = (Heap *) shmat(shmid, NULL, 0);   // Shared Heap
    shared_heap = CreateHeap(100);

    mutex = sem_open("heapSem", O_CREAT | O_EXCL, 0644, ONE); // Semophore

    rand_priority = (int *) malloc(100 * sizeof(int));  // Dynamic Array for storing random priorities

    for (i = 0; i < 100; i++)
        rand_priority[i] = rand() % 50 + 1;

    for (i = 0; i < 100; i++) {
        pid = fork();

        insert(shared_heap, rand_priority[i]);

        if (pid == 0)
            break;
    }

    if (pid > 0) {
        pid = waitpid(-1, NULL, 0);
        while (pid) {
            if (errno == ECHILD)
                break;
            pid = waitpid(-1, NULL, 0);
        }

        print(shared_heap);
        printf(" Parent: All children have exited.\n");

        shmdt(shared_heap);
        shmctl(shmid, IPC_RMID, 0);

        sem_unlink("heapSem");
        sem_close(mutex);
    }
    else {
        sem_wait(mutex); // down

        printf(" Child(%d) enters the critical section.\n", i);

        printf("Priority Key: %d\n", rand_priority[i]);
        print(shared_heap);
        printf(" Child(%d) exits the critical region\n\n", i);

        sem_post(mutex); // up

    }

    exit(0);
}