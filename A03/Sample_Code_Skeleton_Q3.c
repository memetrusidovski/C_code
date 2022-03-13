#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

sem_t *running;
sem_t *even;
sem_t *odd;

void logStart(char *tID);  // function to log that a new thread is started
void logFinish(char *tID); // function to log that a thread has finished its time

void startClock();	   // function to start program clock
long getCurrentTime(); // function to check current time since clock was started
time_t programClock;   // the global timer/clock for the program

typedef struct thread // represents a single thread, you can add more members if required
{
	char tid[4]; // id of the thread as read from file
	unsigned int startTime;
	int state;
	pthread_t handle;
	int retVal;
} Thread;

// you can add more functions here if required

int threadsLeft(Thread *threads, int threadCount);
int threadToStart(Thread *threads, int threadCount);
void *threadRun(void *t);						// the thread function, the code executed by each thread
int readFile(char *fileName, Thread **threads); // function to read the file content and build array of threads

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}

	sem_unlink("/semaphore1");
	sem_unlink("/semaphore2");
	sem_unlink("/semaphore3");
	running = sem_open("/semaphore1", O_CREAT, 0644, 1);
	odd = sem_open("/semaphore2", O_CREAT, 0644, 0);
	even = sem_open("/semaphore3", O_CREAT, 0644, 0);

	// you can add some suitable code anywhere in main() if required

	Thread *threads = NULL;
	int threadCount = readFile(argv[1], &threads);

	startClock();

	if (((int)threads[0].tid[2] - 48) % 2 == 0) sem_post(even);
	else sem_post(odd);
	int c = 0;

	while (threadsLeft(threads, threadCount) > 0) // put a suitable condition here to run your program
	{
		// you can add some suitable code anywhere in this loop if required
		if(c == threadCount){
			sem_post(even);
			sem_post(odd);
		}
		

			int i = 0;
		while ((i = threadToStart(threads, threadCount)) > -1)
		{
			c++;
			// you can add some suitable code anywhere in this loop if required
			threads[i].state = 1;
			threads[i].retVal = pthread_create(&(threads[i].handle), NULL,
											   threadRun, &threads[i]);
		}
		

	}
	return 0;
}

int readFile(char *fileName, Thread **threads) // do not modify this method
{
	FILE *in = fopen(fileName, "r");
	if (!in)
	{
		printf(
			"Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';
	while (!feof(in))
	{
		char line[100];
		if (fgets(line, 100, in) != NULL)
		{
			strncat(fileContent, line, strlen(line));
		}
	}
	fclose(in);

	char *command = NULL;
	int threadCount = 0;
	char *fileCopy = (char *)malloc((strlen(fileContent) + 1) * sizeof(char));
	strcpy(fileCopy, fileContent);
	command = strtok(fileCopy, "\r\n");
	while (command != NULL)
	{
		threadCount++;
		command = strtok(NULL, "\r\n");
	}
	*threads = (Thread *)malloc(sizeof(Thread) * threadCount);

	char *lines[threadCount];
	command = NULL;
	int i = 0;
	command = strtok(fileContent, "\r\n");
	while (command != NULL)
	{
		lines[i] = malloc(sizeof(command) * sizeof(char));
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	for (int k = 0; k < threadCount; k++)
	{
		char *token = NULL;
		int j = 0;
		token = strtok(lines[k], ";");
		while (token != NULL)
		{
			// if you have extended the Thread struct then here
			// you can do initialization of those additional members
			// or any other action on the Thread members
			(*threads)[k].state = 0;
			if (j == 0)
				strcpy((*threads)[k].tid, token);
			if (j == 1)
				(*threads)[k].startTime = atoi(token);
			j++;
			token = strtok(NULL, ";");
		}
	}
	return threadCount;
}

void logStart(char *tID)
{
	printf("[%ld] New Thread with ID %s is started.\n", getCurrentTime(), tID);
}

void logFinish(char *tID)
{
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
}

int threadsLeft(Thread *threads, int threadCount)
{
	int remainingThreads = 0;
	for (int k = 0; k < threadCount; k++)
	{
		if (threads[k].state > -1)
			remainingThreads++;
	}
	return remainingThreads;
}

int threadToStart(Thread *threads, int threadCount)
{
	for (int k = 0; k < threadCount; k++)
	{
		if (threads[k].state == 0 && threads[k].startTime == getCurrentTime())
			return k;
	}
	return -1;
}

void *threadRun(void *t) // implement this function in a suitable way
{
	logStart(((Thread *)t)->tid);

	Thread *temp = (Thread *)t;

	int p = 0;
	int *q = 0;
	//sem_wait(even);
	//sem_getvalue(odd, q);
	//printf("%d<<<< %c>>>>>>>\n",((int)temp->tid[2] - 48), temp->tid[2] );

	if (((int)temp->tid[2] - 48) % 2 == 0)
	{
		sem_wait(even);
	}
	else sem_wait(odd);
		
		

	// sem_wait(odd);
	//  critical section starts here
	sem_wait(running);
	printf("[%ld] Thread %s is in its critical section\n", getCurrentTime(),
		   ((Thread *)t)->tid);
	sem_post(running);
	// critical section ends here
	// sem_post(odd);


	logFinish(((Thread *)t)->tid);
	((Thread *)t)->state = -1;

	if (((int)temp->tid[2] - 48) % 2 != 0)
		sem_post(even);
	else
		sem_post(odd);

	pthread_exit(0);
}

void startClock()
{
	programClock = time(NULL);
}

long getCurrentTime() // invoke this method whenever you want check how much time units passed
// since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return now - programClock;
}
