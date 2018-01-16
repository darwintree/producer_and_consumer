#include <stdio.h>  
#include <pthread.h>  
#include <semaphore.h>  
#include <sys/syscall.h>
  
#define MAX 10  //队列长度 
#define TARGET 50 
  
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
sem_t full;     //填充的个数  
sem_t empty;    //空槽的个数  
  
int top = 0;     //队尾  
int bottom = 0;  //队头  

int total_consume = 0;
  
void* produce(void* arg)  
{  
    int i;  
    for ( i = 0; i < 100; i++)  
    {  
        
	    bool toBreak = false;
        sem_wait(&empty);//若空槽个数低于0阻塞  
          
        pthread_mutex_lock(&mutex);  
        if (total_consume < TARGET) {
	        
            top = (top+1) % MAX;  
	        printf("producer thread %u ", pthread_self()); 
            printf("puts data to position");  
            printf("%d.\n", top);  
	    } else {
            toBreak = true;
        }
  
        pthread_mutex_unlock(&mutex);  
          
        sem_post(&full);  
        if (toBreak) break; 
    }  
    return (void*)1;  
}  
  
void* consume(void* arg)  
{  
    int i;  
    for ( i = 0; i < 100; i++)  
    {  
        bool toBreak = false;
        sem_wait(&full);//若填充个数低于0阻塞  
      
        pthread_mutex_lock(&mutex);  
        if (total_consume < TARGET) {
	        
	        printf("    consumer thread %u ", pthread_self());  
	        printf("consumes data");  
	        printf(" at position %d.", bottom);  
            bottom = (bottom+1) % MAX;  
	        total_consume += 1;
	        printf(" %dth consume\n", total_consume);
	    } else {
            toBreak = true;
        }

	
        pthread_mutex_unlock(&mutex);  
          
        sem_post(&empty);  
        if (toBreak) break;
    }  
  
    return (void*)2;  
}  
  
int main(int argc, char *argv[])  
{  
    pthread_t thid1;  
    pthread_t thid2;  
    pthread_t thid3;  
    pthread_t thid4;
    pthread_t thid5;  
  
    int  ret1;  
    int  ret2;  
    int  ret3;  
    int  ret4;  
    int  ret5;
  
    sem_init(&full, 0, 0);  
    sem_init(&empty, 0, MAX);  
  
    pthread_create(&thid1, NULL, produce, NULL);  
    pthread_create(&thid2, NULL, consume, NULL);  
    pthread_create(&thid3, NULL, produce, NULL);  
    pthread_create(&thid4, NULL, consume, NULL);  
    pthread_create(&thid5, NULL, produce, NULL);  
  
    pthread_join(thid1, (void**)&ret1);  
    pthread_join(thid2, (void**)&ret2);  
    pthread_join(thid3, (void**)&ret3);  
    pthread_join(thid4, (void**)&ret4);  
    pthread_join(thid5, (void**)&ret5);  
    return 0;  
}  
