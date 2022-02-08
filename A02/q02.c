/*
------------------------------------------------------------------------------------------
File:    q02.c
Project: C_code
Purpose:
==========================================================================================

Program Description:
  This program...
------------------------------------------------------------------------------------------
Author:  Memet Rusidovski
ID:      130951550
Email:   rusi1550@mylaurier.ca
Version  2022-02-08
-------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int size = 0;
int *list;

float avg = 0;
int mx = 0;
int mn = 0;
float md = 0;

void *average()
{
    int total = 0;

    for (int i = 0; i < size; i++)
    {
        total += list[i];
    }

    avg = total / size;

    return NULL;
}

void *max()
{
    int max = 0;

    for (int i = 0; i < size; i++)
    {
        if (list[i] > max)
            max = list[i];
    }

    mx = max;

    return NULL;
}

void *min()
{
    int min = list[0];

    for (int i = 1; i < size; i++)
    {
        if (list[i] < min)
            min = list[i];
    }

    mn = min;

    return NULL;
}

void *median()
{
    float median = 0;

    if (size % 2 == 0)
    {
        median = (list[size / 2] + list[size / 2 - 1]) / 2;
    }
    else
    {
        median = list[size / 2];
    }

    md = median;

    return NULL;
}

int main(int argc, char *argv[])
{
    //printf("%d %d \n", argc, atoi(argv[0]));

    int lst[argc - 1];
    for (int i = 1; i < argc; i++)
        lst[i-1] = atoi(argv[i]);
    //  int lst []= {98, 100, 102, 43, 43, 54, 64, 12, 43, 54, 11, 23};
    list = lst;
    size = (int)(sizeof(lst) / sizeof(lst[0]));

    pthread_t thread_id;
    pthread_t thread_id2;
    pthread_t thread_id3;
    pthread_t thread_id4;

    pthread_create(&thread_id, NULL, &average, NULL);
    pthread_create(&thread_id2, NULL, &max, NULL);
    pthread_create(&thread_id3, NULL, &min, NULL);
    pthread_create(&thread_id4, NULL, &median, NULL);
    pthread_join(thread_id, NULL);
    pthread_join(thread_id2, NULL);
    pthread_join(thread_id3, NULL);
    pthread_join(thread_id4, NULL);

    printf("average: %.5f\nmax: %d\nmin: %d \nmedian: %.2f\n", avg, mx, mn, md);

    return 0;
}