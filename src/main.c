#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "crc/crc.h"

#define DEBUG 1

typedef struct
{
    char Adr[8];
    char Control[8];
    char Data[32];
    char FCS[16];
} Trame;

typedef struct
{
    char fanionStart[8];
    Trame trame;
    char fanionEnd[8];
} TrameAll;

int PIPEDES[2];
char fanion[] = "01111110";
char Generator[] = "10001000000100001";
// char Generator[] = "10011";
void addMessage(char *data, Trame *trameToSend);
void createTrame(char *data, Trame *trameToSend);
Trame *receiveTrame();
void sendTrame(Trame *trameToSend);
void DebugTrameAll(TrameAll *trameA, char *str);
void DebugTrame(Trame *trame);
Trame *Decode(Trame *trame);
Trame *Encode(Trame *trame);
/**
 * @brief Le processus fils contacte le processus pére en utlisant le protocole HDLC
 * le fils : transmet un message sous forme de trame (il doit la creer)
 * le pere : reçois via les pipes la trame HDLC et la traite pour detecter d'eventuelles erreurs
 *
 */
int main(int argc, char const *argv[])
{

    int resPipe = pipe(PIPEDES);

    if (!resPipe)
    {
        int resFork = fork();
        if (resFork > 0)
        {
            if (DEBUG == 1)
            {
                printf("Press Any Key to Continue\n");
                getchar();
            }

            // Pere

            Trame *trameRead = receiveTrame();
        }
        else if (resFork == 0)
        {
            if (DEBUG == 1)
            {
                printf("Press Any Key to Continue\n");
                getchar();
            }
            // fils

            Trame *trameToSend = malloc(sizeof(Trame));

            char *data = "01101000011000010110110101111010";
            // char *data = "01101000011000010110110101111010";

            addMessage(data, trameToSend);
            sendTrame(trameToSend);
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

    printf("\n");

    return 0;
}

void addMessage(char *data, Trame *trameToSend)
{
    /**
     * @brief NEED to handle case where input data is less or more than 32
     *
     */
    // strncpy(trameToSend->Data, data, strlen(data) * sizeof(char));
    strncpy(trameToSend->Data, data, strlen(data) * sizeof(char));
    strncpy(trameToSend->FCS, CRC2(data, Generator, strlen(data), strlen(Generator)), 16 * sizeof(char));
}

void sendTrame(Trame *trameToSend)
{
    trameToSend = Encode(trameToSend);
    TrameAll *Write = malloc(sizeof(TrameAll));
    strncpy(Write->fanionStart, fanion, 8 * sizeof(char));
    Write->trame = *trameToSend;

    if (DEBUG == 1)
    {
        DebugTrameAll(Write, "SENT");
    }
    write(PIPEDES[1], Write, sizeof(TrameAll));
}

Trame *receiveTrame()
{
    TrameAll *Read = malloc(sizeof(TrameAll));
    read(PIPEDES[0], Read, sizeof(TrameAll));
    Read->trame = *Decode(&(Read->trame));
    if (DEBUG == 1)
    {
        DebugTrameAll(Read, "RECEIVED");
    }
    return &(Read->trame);
}

void DebugTrameAll(TrameAll *trameA, char *str)
{
    if (str != NULL)
    {
        printf("\n##################[%s]######################\n", str);
    }
    else
    {
        printf("\n########################################\n");
    }
    printf("\n\tFanionStart: %s", trameA->fanionStart);
    DebugTrame(&(trameA->trame));
    printf("\n\tFanionEnd: %s", trameA->fanionStart);
    printf("\n########################################\n");
}

void DebugTrame(Trame *trame)
{
    printf("\n\tAdress: %s\n\tControl:%s\n\tData:%s\n\tFCS:%s", trame->Adr, trame->Control, trame->Data, trame->FCS);
}

/**
 * @brief toute suite de 5 bits consécutifs à 1 est transcodée en une suite de cinq
bits à 1 et d'un bit à 0.
 *
 * @return Trame*
 */
Trame *Encode(Trame *trame)
{
    return trame;
}

/**
 * @brief  opération inverse de Encode
 *
 * @return Trame*
 */
Trame *Decode(Trame *trame)
{
    return trame;
}