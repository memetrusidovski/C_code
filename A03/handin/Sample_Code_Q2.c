#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <semaphore.h>
// Global Data variables.
int a = 5, b = 7;
sem_t *mutex;

// Function that access the global data.
void* inc_dec(void *arg) {
	sem_wait(mutex);

	printf("Read value of 'a' global variable is: %d\n", a);
	printf("Read value of 'b' global variable is: %d\n", b);
	sleep(1);
	
	a = a + 1;
	b = b - 1;
	
	printf("Updated value of 'a' variable is: %d\n", a);
	printf("Updated value of 'b' variable is: %d\n", b);

	sem_post(mutex);

	return 0;
}
int main() {
	//sem_init(&mutex, 0, 1);
	if ((mutex = sem_open("/semaphore", O_CREAT, 0644, 1)) == SEM_FAILED)
	{
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
	// Creating the thread instances.
	pthread_t t1,
	t2, t3;
	pthread_create(&t1, NULL, inc_dec, NULL);
	pthread_create(&t2, NULL, inc_dec, NULL);
	pthread_create(&t3, NULL, inc_dec, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	//sem_destroy(&mutex);

	//Destroying the threads.
	pthread_exit(t1);
	pthread_exit(t2);
	pthread_exit(t3);
	return 0;
}
