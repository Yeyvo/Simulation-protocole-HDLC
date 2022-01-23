#ifndef UTILSINC
#define UTILSINC
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0

char* intToBinaryChar(int n);
void substring(char s[], char sub[], int p, int l);
char* intToBinaryCharPadded(int n, int sz);

#endif 