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
char *strrev(char *str);
char * int2bin(int i);
char *toBin(long long int num);
int toDec(char bin[], int len);
int equals(char s1[], char s2[]);
#endif 