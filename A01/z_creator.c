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

int main()
{

  pid_t child_pid = fork();

  if (child_pid > 0){
    printf("Hello World %i", child_pid);
    sleep(10);

  }

  else
    exit(0);

  return 0;
}