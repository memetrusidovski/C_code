/*
------------------------------------------------------------------------------------------
File:    thread.c
Project: C_code
Purpose: 
==========================================================================================

Program Description:
  This program...
------------------------------------------------------------------------------------------
Author:  Memet Rusidovski
ID:      130951550
Email:   rusi1550@mylaurier.ca
Version  2022-02-07
-------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <pthread.h>

void *multiThread(void *vargp)
{
    printf("Custom thread created successfully\n");

    for (int i = 0; i < 4; i++)
    {
        printf("I am a custom thread function created by the user.\n");
        sleep(1);
    }
    
    return NULL;
}

int main(void){
    printf("This program would create threads\n");
    
    
    pthread_t thread_id;
    pthread_t thread_id2;

    pthread_create(&thread_id, NULL, &multiThread, NULL);
    pthread_create(&thread_id2, NULL, &multiThread, NULL);
    pthread_join(thread_id, NULL);
    pthread_join(thread_id2, NULL);

    for(int i = 0; i < 4; i++){
        printf("I am the process thread created by the complier by default.\n");
        sleep(1);
    }

    return 0;
}