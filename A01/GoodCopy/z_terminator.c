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
    int pid;

    pid = system("kill -9 $(ps -e -l | grep -w Z | tr -s ' ' | cut -d ' ' -f 4)");

    return 0;
}