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

#define PAGESIZE 4096

// Print out put, Prints nothing if command doesnt exist
void writeOutput(char *command, char *output)
{
    FILE *fp;
    fp = fopen("output.txt", "a");
    fprintf(fp, "The output of: %s : is\n", command);
    fprintf(fp, ">>>>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<<\n", output);

    fclose(fp);
}

int readFile(char *name, char (*commands)[100], int *count)
{
    char const *const fileName = name; /* should check that argc > 1 */
    FILE *file = fopen(fileName, "r"); /* should check the result */
    char line[256];
    int c = 0;

    // Get all file commands
    while (fgets(line, sizeof(line), file))
    {
        strncpy(commands[c], line, 100);
        c++;
    }

    fclose(file);
    *count = c;

    return 0;
}

int main()
{
    key_t key = ftok("test.txt", 0);
    int shmid = shmget(key, PAGESIZE, 0644 | IPC_CREAT);
    char(*data)[100] = (char(*)[100])shmat(shmid, NULL, 0);

    // Shared memory bank
    int *shared_count = mmap(NULL, PAGESIZE,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char(*shared_memory)[100] = (char(*)[100])mmap(NULL, PAGESIZE,
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int child_id = fork();
    int fd[2];
    pipe(fd);

    if (child_id == 0)
    {
        int pipe_id = fork();
        // Create the fork that finds the commands
        if (pipe_id == 0)
        {
            // Read the commands and save them to shared memory
            readFile("sample_in.txt", shared_memory, shared_count);
            for (int i = 0; i < *shared_count; i++)
                printf("%s", shared_memory[i]);
        }
        // This fork is for running the commands once the previous one finds them
        else
        {
            wait(NULL);
            // Create a pipe for each command
            int pipes[*shared_count + 1][2];
            int run_id[*shared_count];

            //  Write the commands to pipes
            for (int i = 0; i < *shared_count; i++)
            {
                if (pipe(pipes[i]) == -1)
                    printf("Error Occurred");

                char *x = shared_memory[i];
                write(pipes[i][1], x, sizeof(char) * 256);
            }

            for (int i = 0; i < *shared_count; i++)
            {
                run_id[i] = fork();

                // Create a fork for each command and pipe to it to pass it a command
                if (run_id[i] == 0)
                {
                    char tempS[256];
                    char output[256];
                    read(pipes[i][0], tempS, 256);
                    printf("Im a run ID: %s \n", tempS);

                    FILE *cmd = popen(tempS, "r");
                    char result[256] = {0x0};
                    fgets(result, sizeof(result), cmd);
                    printf("Output: %s\n", result);
                    pclose(cmd);

                    writeOutput(tempS, result);

                    return 0;
                }
            }

            for (int i = 0; i < *shared_count; i++)
                wait(NULL);
        }
    }
    // Kill unused processes
    else
    {
        return 0;
    }

    return 0;
}
