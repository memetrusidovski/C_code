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

#define PAGESIZE 4096

void writeOutput(char *command, char *output)
{
    FILE *fp;
    fp = fopen("output.txt", "a");
    fprintf(fp, "The output of: %s : is\n", command);
    fprintf(fp, ">>>>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<<\n", output);

    fclose(fp);
}

int readFile(char *name, char **commands)
{
    char const *const fileName = name; /* should check that argc > 1 */
    FILE *file = fopen(fileName, "r"); /* should check the result */
    char line[256];
    int c = 0;

    while (fgets(line, sizeof(line), file))
    {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%s", line);
        commands[c] = malloc(sizeof(line)+1);
        strcpy(commands[c], line);
        c++;
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);

    return 0;
}

int saveToMemory()
{

    return 0;
}

int main()
{
    // Shared memory bank
    char **shared_memory = mmap(NULL, PAGESIZE,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int child_id = fork();
    int fd[2];
    pipe(fd);

    *shared_memory = "Th string";

    if (child_id != 0)
    {
        int pipe_id = fork();
        if (pipe_id != 0)
        {
            readFile("sample_in.txt", shared_memory);
            int x = 50;
            //*shared_memory = "New string";
        printf("%s<++++", shared_memory[0] );
            close(fd[0]);
            write(fd[1], &x, sizeof(int));
            close(fd[1]);
        }
        else
        {
            int y;
            close(fd[1]);
            read(fd[0], &y, sizeof(int));
            close(fd[0]);
            printf("%d<====\n", y);
        }
    }
    else
    {
        wait(NULL);
    }

    printf("%d\n", getpid());

    return 0;
}
