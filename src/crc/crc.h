#if !defined(CRCINC)
#define CRCINC
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include "../utils/utils.h"
#endif 


void append(char s[], char c);
void charToBinary(char c, int binary[]);
char binaryToChar(char binary[], int octect);
void applyXor (char result[], char G[], char temp[]);
void ajouterZero(char message[], int n);
void extraire(char from[], char to[], int n, int debut);
char* CRC(char msg[] );
void decaler(int m, char from[], char to[]);
int equals(char s1[], char s2[]);


char *strrev(char *str);
char *toBin(long long int num);
int toDec(char bin[], int len);
char * CRC2(char dataword[], char generator[], int lenData, int lenGene);
// char *int2bin(int a, char *buffer, int buf_size);
char * int2bin(int i);
// char *intToBinaryChar(int n);