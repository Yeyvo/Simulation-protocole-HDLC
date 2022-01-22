

// Concatenate char to String
void append(char s[], char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
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
void CRC(char msg[], char temp[])
{
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

void test(int n){
    printf("\n##3 %d" , n);
}