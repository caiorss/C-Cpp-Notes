/*
Description: Show glibc Version. 
OS:          Linux only 

Compile with:

 $ gcc glibc-version.c -o glibc-version.bin && ./glibc-version.bin 
 glibc version: 2.24

 */
#include <stdio.h>
#include <stdlib.h>
#include <gnu/libc-version.h>

int main(int argc, char *argv[]) {
  printf("glibc version: %s\n", gnu_get_libc_version());
}

