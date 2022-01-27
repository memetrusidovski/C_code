/*
------------------------------------------------------------------------------------------
File:    z_creator.c
Project: A01
Purpose:
==========================================================================================

Program Description:
  This program...
------------------------------------------------------------------------------------------
Author:  Memet Rusidovski
ID:      130951550
Email:   rusi1550@mylaurier.ca
Version  2022-01-23
-------------------------------------
*/

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

#define PAGESIZE 40960

void writeOutput(char *command, char *output)
{
    FILE *fp;
    fp = fopen("output.txt", "a");
    fprintf(fp, "The output of: %s : is\n", command);
    fprintf(fp, ">>>>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<<\n", output);

    fclose(fp);
}

int readFile(char *name, char **commands, int *count)
{
    char const *const fileName = name; /* should check that argc > 1 */
    FILE *file = fopen(fileName, "r"); /* should check the result */
    char line[256];
    int c = 0;

    char **tempPoint = (char **)malloc(1000);

    while (fgets(line, sizeof(line), file))
    {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%s", line);
        // char *temp = line;
        // fflush(stdout);

        commands[c] = strdup(line);
        //*(commands + c) = line;
        //*(tempPoint + c) = strdup(line);
        //*(commands +c) = strdup(line);

        //*(commands + c) = "line";
        c++;
    }

    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);

    // count = malloc(sizeof(int)+2);
    // for (int i = 0; i < 5; i++)
    //  *(commands + i) = strdup(*(tempPoint + i));

    *count = c;
    return 0;
}

int main()
{
    // Shared memory bank
    int key = shm_open("/sample_in.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(key, 4096);

    int *shared_count = mmap(NULL, PAGESIZE,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char **shared_memory = mmap(NULL, PAGESIZE,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int child_id = fork();
    int fd[2];
    pipe(fd);

    // shared_memory = (char**) malloc(10000);
    //*shared_count = 3;

    if (child_id == 0)
    {
        int pipe_id = fork();
        if (pipe_id == 0)
        {

            readFile("sample_in.txt", shared_memory, shared_count);
            for (int i = 0; i < *shared_count; i++)
            {
                printf("%s??????????", shared_memory[i]);
            }
            //char **x = strcpy(shared_memory);
            // *shared_memory = "New string";
            //printf("+++>%s<++++", *shared_memory);

            close(fd[0]);
            write(fd[1], shared_memory, sizeof(shared_memory));
            close(fd[1]);
        }
        else
        {
            int pipes[*shared_count + 1][2];
            int run_id[*shared_count];

            printf("%d <><>< > %s \n", *shared_count, *shared_memory);
            // if(fork() ==0)
            //  Write the commands to pipes
            for (int i = 0; i < 3; i++)
            {
                if (pipe(pipes[i]) == -1)
                {
                    printf("Error Occurred");
                }

                char x[] = "STringshared_memory[i]";
                write(pipes[i][1], x, sizeof(char) * 256);
            }

            // else{
            //  wait(NULL);

            for (int i = 0; i < 3; i++)
            {
                run_id[i] = fork();

                if (run_id[i] == 0)
                {
                    /*for(int j = 0; j < 3+1; j++)
                    {
                        if(i != j){
                            close(pipes[j][0]);

                        }
                        if(i + 1 != j){
                            close(pipes[j][1]);
                        }
                    }*/

                    char tempS[256];
                    read(pipes[i][0], tempS, 256);
                    printf("Im a run ID: %s \n", tempS);
                    printf("TESTING FORK:\n");

                    return 0;
                }
            }

            for (int i = 0; i < 3; i++)
            {
                wait(NULL);
            }
            char **y = malloc(4096);
            close(fd[1]);
            read(fd[0], y, sizeof(y));
            close(fd[0]);
            printf("%s<====\n", y[0]);

            //}
        }
    }
    else
    {
        printf("%sTHELOOP", shared_memory[0]);
        return 0; // wait(NULL);
    }

    printf("%s?\n", shared_memory[2]);
    return 0;
}
