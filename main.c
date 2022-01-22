#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/wait.h>
#include <string.h>
#include "crc/crc.h"

typedef struct
{
    char Adr[8];
    char Control[8];
    char Data[32];
    char FCS[16];
} Trame;

int PIPEDES[2];

void addMessage(char *data, Trame *trameToSend);
void createTrame(char *data, Trame *trameToSend);

/**
 * @brief Le processus fils contacte le processus pére en utlisant le protocole HDLC
 * le fils : transmet un message sous forme de trame (il doit la creer)
 * le pere : reçois via les pipes la trame HDLC et la traite pour detecter d'eventuelles erreurs
 *
 */
int main(int argc, char const *argv[])
{
    test(7);
    int resPipe = pipe(PIPEDES);

    if (!resPipe)
    {
        int resFork = fork();
        if (resFork > 0)
        {
            // Pere
            // Trame *trameRead = malloc(sizeof(Trame));
            // write(PIPEDES[0], trameRead, sizeof(Trame));
        }
        else if (resFork == 0)
        {
            // fils
            // int addressePere = getppid();
            // printf("fils");

            Trame *trameToSend = malloc(sizeof(Trame));

            // char *data = "1001010001111010101010101010010101010100101010101010101111100000";

            // addMessage(data, trameToSend);

            // write(PIPEDES[1], trameToSend, sizeof(Trame));
        }
        else
        {
            printf("\n**Erreur FORK**\n");
        }
    }
    else
    {
        printf("\n**Erreur PIPE**\n");
    }

    //  while (wait(NULL))
    //             {
    //             }
    //             printf("\n**CLOSE PIPE**\n");
    //             close(PIPEDES[0]);
    //             close(PIPEDES[1]);
    return 0;
}

void addMessage(char *data, Trame *trameToSend)
{
    for (int i = 0; i < strlen(data) / 32; i++)
    {
        char *effectiveData;
        strncat(effectiveData, data + i * 32, 32);
        createTrame(effectiveData, trameToSend);
    }
}

void createTrame(char *data, Trame *trameToSend)
{
    strncpy(trameToSend->Data, data, 32);
}
