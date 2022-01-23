#include "utils.h"
 
char* intToBinaryChar(int n) /* Function to convert decimal to binary.*/
{
    int remainder;
    long long int i = 1;
    long long int binaryRepresentation = 0;
    while (n != 0)
    {
        remainder = n % 2;
        n /= 2;
        binaryRepresentation += remainder * i;
        i *= 10;
    }
    // char *str = malloc(32*sizeof(char));
    char *str = calloc(16,sizeof(char)) ;
    sprintf(str, "%lld", binaryRepresentation);
    return str;
}

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}
char* intToBinaryCharPadded(int n, int sz){
    char * bc = intToBinaryChar(n);
    char *res = (char *) calloc(sz,sizeof(char)) ;
    int maxPad = sz - strlen(bc);
    for(int i = 0, z = 0 ; i< sz; i++){
        if(i<maxPad){
            res[i] = '0';
        }else{
            res[i] = bc[z];
            z++;
        }
    }
    // res[sz] = '\0';
    return res;
}


char *strrev(char *str)
{
    char c, *front, *back;

    if (!str || !*str)
        return str;
    for (front = str, back = str + strlen(str) - 1; front < back; front++, back--)
    {
        c = *front;
        *front = *back;
        *back = c;
    }
    return str;
}


char *int2bin(int i)
{
    size_t bits = sizeof(int) * (sizeof(char) * 8);

    char *str = malloc(bits + 1);
    if (!str)
        return NULL;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for (; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    return str;
} 


/**
 * @brief return 1 if s1 equals s2
 * 
 * @param s1 
 * @param s2 
 * @return int 
 */
int equals(char s1[], char s2[])
{
    int compt = 0;
    int egaux = 1;
    while (compt < strlen(s1))
    {
        if (s1[compt] != s2[compt])
        {
            egaux = 0;
        }
    }
    return egaux;
}

/**
 * @brief function to convert integer to binary string
 * 
 * @param num 
 * @return char* 
 */
char *toBin(long long int num)
{
    char *bin = malloc(sizeof(long long int) * 8);
    while (num)
    {
        if (num & 1)
        {
            // bin = "1" + bin;
            strcat(bin, "1");
        }
        else
        {
            // bin = "0" + bin;
            strcat(bin, "0");
        }
        num = num >> 1;
    }

    return strrev(bin);
}

/**
 * @brief function to convert binary string to decimal
 * 
 * @param bin 
 * @param len 
 * @return int 
 */
int toDec(char bin[], int len)
{
    int num = 0;
    for (int i = 0; i < len; i++)
    {
        if (bin[i] == '1')
            num += 1 << (len - i - 1);
    }
    return num;
}