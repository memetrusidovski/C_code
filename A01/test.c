
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdint.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PAGESIZE 4096

int main()
{
    // Shared memory bank
    //int key = shm_open("sample_in.txt", O_CREAT | O_RDWR, 0666);
    key_t key = ftok("test.txt", 0);
        //ftruncate(key, 4096);

    int shmid = shmget(key, PAGESIZE, 0644 | IPC_CREAT);
    char (*data)[100] =(char (*)[100]) shmat(shmid, NULL, 0);

    int *shared_count = mmap(NULL, PAGESIZE,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char (*shared_memory)[100] = (char(*)[100]) mmap(0, PAGESIZE,
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int child_id = fork();
    int fd[2];
    pipe(fd);

    // shared_memory = (char**) malloc(10000);
    //*shared_count = 3;
    if (child_id == 0)
    {
        strncpy(shared_memory[0], "YOOOOO", PAGESIZE);

        //sprintf(*shared_memory, "%s", "World");
        //strcpy(shared_memory[0], "World");

            key_t key = ftok("test.txt", 0);
            // ftruncate(key, 4096);
            int shmid = shmget(key, PAGESIZE, 0644 | IPC_CREAT);
            char(*data)[100] = (char(*)[100])shmat(shmid, NULL, 0);
            strncpy(data[0], "YOOOOO", PAGESIZE);
            printf("run");
    }
    else{

        if(fork()==0){
            printf("test");
        }else{
            wait(NULL);
            key_t key = ftok("test.txt", 0);
            // ftruncate(key, 4096);

            int shmid = shmget(key, PAGESIZE, 0644 | IPC_CREAT);
            char(*data)[100] = (char(*)[100])shmat(shmid, NULL, 0);
            printf("%s>\n", data[0]);

            printf("%s?\n", shared_memory[0]);
        }
    }
        

    return 0;
}
