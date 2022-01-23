#include "crc.h"

// Concatenate char to String
void append(char *str, char c)
{
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}
// function to convert char into binary list
void charToBinary(char c, int binary[])
{
    int j = 0;
    int i = (int)c;
    while (j < 8)
    {
        binary[j] = i % 2;
        i = i / 2;
        j++;
    }
}
// function to convert binary list to char
char binaryToChar(char binary[], int octect)
{
    int k = 0;
    int i = 0;
    while (k < 8)
    {
        if (binary[k + 8 * octect] == '1')
        {
            i = i + pow(2, 7 - k);
        }
        k++;
    }
    char c = (char)i;
    return c;
}
// Fonction qui réalise le XOR entre 2 suite de bit et met le résultat dans 'result'
void applyXor(char result[], char G[], char temp[])
{
    int curseur = 0;
    while (curseur < strlen(G))
    {
        if (G[curseur] == temp[curseur])
            result[curseur] = '0';
        else
            result[curseur] = '1';
        curseur++;
    }
    result[curseur] = '\0';
}
// Fonction qui ajoute 16 bit a zero a la fin d'un message binaire
void ajouterZero(char message[], int n)
{
    int compt = 0;
    while (compt < n)
    {
        append(message, '0');
        compt++;
    }
}

// Fonction d'extraction de n bit, et les mettre dans un char
void extraire(char from[], char to[], int n, int debut)
{
    int compt = 0;
    while (compt < n)
    {
        to[compt] = from[compt + debut];
        compt++;
    }
    to[compt] = '\0';
}
// Fonction qui réalise un décalage de 1 bit, et met le résultat dans to de taille m (= 17)
void decaler(int m, char from[], char to[])
{
    int compt = 0;
    while (compt < m)
    {
        to[compt] = from[compt + 1];
        compt++;
        to[compt] = '\0';
    }
}

// Fonction qui calcule le crc et le met dans temp
char *CRC(char msg[])
{
    char temp[16];
    char G[] = "10001000000100001";
    char result[strlen(G) + 1];
    ajouterZero(msg, strlen(G) - 1);
    extraire(msg, result, strlen(G), 0);
    int i; // indice du bit a rajouter
    if (result[0] == '0')
    {
        decaler(strlen(G), result, temp);
        append(temp, msg[strlen(G)]);
        i = 1;
    }
    else if (result[0] == '1')
    {
        applyXor(result, result, G);
        decaler(strlen(G), result, temp);
        i = 0;
    }
    i += strlen(G);
    while (i < 8 + 16)
    {

        append(temp, msg[i]);
        if (temp[0] == '0')
        {
            decaler(strlen(G), temp, temp);
        }
        else
        {
            applyXor(result, temp, G);
            decaler(strlen(G), result, temp);
        }
        i++;
    }
    return temp;
}

// FOnction qui renvoie 1 si deux string sont egaux
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

// function to convert integer to binary string
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

// function to convert binary string to decimal
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

// function to compute CRC and codeword
char *CRC2(char dataword[], char generator[], int lenData, int lenGene)
{
    int l_gen = lenGene;
    int l_data = lenData;
    int gen = toDec(generator, lenGene);

    int dword = toDec(dataword, l_data);

    // append 0s to dividend
    int dividend = dword << (l_gen - 1);

    // shft specifies the no. of least
    // significant bits not being XORed
    int shft = (int)ceill(log2l(dividend + 1)) - l_gen;
    int rem;

    while ((dividend >= gen) || (shft >= 0))
    {

        // bitwise XOR the MSBs of dividend with generator
        // replace the operated MSBs from the dividend with
        // remainder generated
        rem = (dividend >> shft) ^ gen;
        dividend = (dividend & ((1 << shft) - 1)) | (rem << shft);

        // change shft variable
        shft = (int)ceill(log2l(dividend + 1)) - l_gen;
    }

    // finally, AND the initial dividend with the remainder (=dividend)
    int codeword = (dword << (l_gen - 1)) | dividend;
    // cout << "Remainder: " << toBin(dividend) << endl;
    // cout << "Codeword : " << toBin(codeword) << endl;
    // return toBin(codeword);

    // buffer[sizeof(long long int) * 8 - 1] = '\0';

    // char buffer[15] = {0};

    // char buffer[33];
    // buffer[33 - 1] = '\0';
    
    return intToBinaryChar(dividend);
    // return int2bin(dividend, buffer, 33 - 1);
}

// char* intToBinaryChar(int n) /* Function to convert decimal to binary.*/
// {
//     int remainder;
//     long long int i = 1, j = 1;
//     long long int binaryRepresentation = 0;
//     while (n != 0)
//     {
//         remainder = n % 2;
//         n /= 2;
//         binaryRepresentation += remainder * i;
//         i *= 10;
//     }
//     // char *str = malloc(32*sizeof(char));
//     char *str = calloc(16,sizeof(char)) ;
//     sprintf(str, "%lld", binaryRepresentation);
//     return str;
// }

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

// buffer must have length >= sizeof(int) + 1
// Write to the buffer backwards so that the binary representation
// is in the correct order i.e.  the LSB is on the far right
// instead of the far left of the printed string
// char *int2bin(int a, char *buffer, int buf_size)
// {
//     buffer += (buf_size - 1);

//     for (int i = 31; i >= 0; i--)
//     {
//         *buffer-- = (a & 1) + '0';

//         a >>= 1;
//     }
//     printf("***********%s",buffer);
//     return buffer;
// }

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
// #define BUF_SIZE 33

// int main() {
//     char buffer[BUF_SIZE];
//     buffer[BUF_SIZE - 1] = '\0';

//     int2bin(0xFF000000, buffer, BUF_SIZE - 1);

//     printf("a = %s", buffer);
// }