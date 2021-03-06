#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

void logStart(char *tID);  // function to log that a new thread is started
void logFinish(char *tID); // function to log that a thread has finished its time

void startClock();	   // function to start program clock
long getCurrentTime(); // function to check current time since clock was started
time_t programClock;   // the global timer/clock for the program

int count = 0;

typedef struct thread // represents a single thread
{
	char *tid; // id of the thread as read from file

	// Added
	int startTime;
	int lifeTime;

} Thread;

void *threadRun(void *t);						// the thread function, the code executed by each thread
int readFile(char *fileName, Thread **threads); // function to read the file content and build array of threads

int main(int argc, char *argv[])
{
	/*if (argc < 2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}*/

	char *name = "file.txt";
	Thread **threads = (Thread **)malloc(sizeof(Thread));

	readFile(name, threads);

	int len = count;
	startClock();
	long time = getCurrentTime();
	int run = 0;
	pthread_t *lastThread = (pthread_t *) malloc(sizeof(pthread_t) * count);

	while(time < 100 && run < count){
		//This is the time step
		//Find threads with a start time right now
		int j = 1;
		Thread temp;
		temp = *threads[0];
		
		while (j <= len) // put a suitable condition here to run your program
		{	
			if(temp.startTime == time){
				pthread_t thread_id;
				//Deep Copy
				lastThread[run] = thread_id;
				
				run++;
				
				//Create the thread
				Thread *t = malloc(sizeof(int));
				*t = temp;
				pthread_create(&thread_id, NULL, &threadRun, t);
			}
			//gcc Question3_Code_Skeleton.c -o q03 && ./q03
			if(j < len) temp = *threads[j];
			j++;
		}
		
		time = getCurrentTime();
		sleep(1);
	}

	//Make sure all the threads are done before terminating
	//for(int i = 0; i < count; i++)
		pthread_join(lastThread[1], NULL);

	return 0;
}

int readFile(char *fileName, Thread **threads) // use this method in a suitable way to read file
{

	FILE *in;
	in = fopen("file.txt", "r");

	if (!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
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

	count = threadCount;

	for (int k = 0; k < threadCount; k++)
	{
		char *token = NULL;
		int j = 0;
		token = strtok(lines[k], ";");
		Thread *temp = (Thread *)malloc(sizeof(Thread));
		while (token != NULL)
		{
			// this loop tokenizes each line of input file
			// write your code here to populate instances of Thread to build a collection
			if (j == 0) temp->tid = strdup(token);
			else if(j == 1)
				temp->startTime = atoi(token);
			else if (j == 2)
				temp->lifeTime = atoi(token);

			printf("%s ", token);
			token = strtok(NULL, ";");
			j++;
		}
		threads[k] = temp;
		printf("\n");
	}
	return threadCount;
}

void logStart(char *tID) // invoke this method when you start a thread
{
	printf("[%ld] New Thread with ID %s is started.\n", getCurrentTime(), tID);
}

void logFinish(char *tID) // invoke this method when a thread is over
{
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
}

void *threadRun(void *t) // implement this function in a suitable way
{
	Thread temp = *((Thread*) t);
	logStart(temp.tid);

	long time = getCurrentTime();
	
	while(time <= temp.lifeTime + temp.startTime){
		sleep(1);
		time = getCurrentTime();
	}

	logFinish(temp.tid);

	return NULL;
}

void startClock() // invoke this method when you start servicing threads
{
	programClock = time(NULL);
}

long getCurrentTime() // invoke this method whenever you want to check how much time units passed
// since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return now - programClock;
}
