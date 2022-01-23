#include "utils.h"
 
char* intToBinaryChar(int n) /* Function to convert decimal to binary.*/
{
    int remainder;
    long long int i = 1, j = 1;
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