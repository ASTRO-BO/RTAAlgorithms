

// binary_semaphore.c
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX 5

int buffer[MAX];
int fill = 0;
int use  = 0;


void put(int value) {
	buffer[fill] = value;
	fill = (fill + 1) % MAX;
}

int get() {
	int b = buffer[use];
	use = (use + 1) % MAX;
	return b;
}

int loops = 0;

sem_t* empty;
sem_t* full;
pthread_mutex_t mutex;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		
		sem_wait(empty);
		
		// scope of lock reduced
		pthread_mutex_lock(&mutex);
		put(i);
		pthread_mutex_unlock(&mutex);
		
		sem_post(full);
	}
}

void *consumer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		sem_wait(full);
		
		// scope of lock reduced
		pthread_mutex_lock(&mutex);
		int b = get();
		//int b = 0;
		pthread_mutex_unlock(&mutex);
		
		sem_post(empty);
		printf("%d\n", b);
	}
}

int main2(int argc, char *argv[])
{
	
	if(argc < 2 ){
		printf("Needs 2nd arg for loop count variable.\n");
		return 1;
	}
	
	loops = atoi(argv[1]);
	
	empty = sem_open("/tmp/empty", O_CREAT, 0644, MAX);
	
	//sem_open(&empty, 0, MAX); // MAX buffers are empty to begin with...
	
	//sem_open(&full, 0, 0);    // ... and 0 are full
	full = sem_open("/tmp/full", O_CREAT, 0644, 0);
	
	pthread_mutex_init(&mutex, NULL);    // mutex = 1 since it a lock
	
	pthread_t pThread, cThread;
	pthread_create(&pThread, 0, producer, 0);
	pthread_create(&cThread, 0, consumer, 0);
	pthread_join(pThread, NULL);
	pthread_join(cThread, NULL);
	sem_close(empty);
	sem_close(full);
	return 0;
}
