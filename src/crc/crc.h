#if !defined(CRCINC)
#define CRCINC
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#endif 


void append(char s[], char c);
void charToBinary(char c, int binary[]);
char binaryToChar(char binary[], int octect);
void applyXor (char result[], char G[], char temp[]);
void ajouterZero(char message[], int n);
void extraire(char from[], char to[], int n, int debut);
void CRC(char msg[], char temp[]);
void decaler(int m, char from[], char to[]);
int equals(char s1[], char s2[]);
void test(int n);