#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "crc/crc.h"
#include "utils/utils.h"

#define DEBUG 1

typedef struct
{
    char Adr[8];
    char Control[8];
    char Data[33];
    char FCS[16];
} Trame;

typedef struct
{
    char fanionStart[9];
    Trame trame;
    char fanionEnd[9];
} TrameAll;

typedef struct
{
    int NS;
    int NR;
} SendReceive;
typedef enum
{
    I,
    S,
    U
} ControlType;

int PIPEDES[2];
char fanion[] = "01111110";
char Generator[] = "10001000000100001";
// char Generator[] = "10011";
void addMessage(char *data, Trame *trameToSend);
void createTrame(char *data, Trame *trameToSend);
Trame *receiveTrame(SendReceive *number);
void sendTrame(Trame *trameToSend, SendReceive *number);
void DebugTrameAll(TrameAll *trameA, char *str);
void DebugTrame(Trame *trame);
Trame *Decode(Trame *trame);
Trame *Encode(Trame *trame);
SendReceive *createSendReceive();
void UpdateNR(SendReceive *dataSendReceive);
void UpdateNS(SendReceive *dataSendReceive);
char *createControl(ControlType type, SendReceive *dataSendReceive, int pool, int C1[2], int C2[3]);
/**
 * @brief Le processus fils contacte le processus pére en utlisant le protocole HDLC
 * le fils : transmet un message sous forme de trame (il doit la creer)
 * le pere : reçois via les pipes la trame HDLC et la traite pour detecter d'eventuelles erreurs
 *
 */
int main(int argc, char const *argv[])
{
    // SendReceive *dataSR = createSendReceive();
    // dataSR->NS = 3;
    // dataSR->NR = 5;
    // createControl(I, dataSR, 1,NULL, NULL);

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
            SendReceive *dataSR = createSendReceive();

            Trame *trameRead = receiveTrame(dataSR);
        }
        else if (resFork == 0)
        {
            if (DEBUG == 1)
            {
                printf("Press Any Key to Continue\n");
                getchar();
            }
            // fils
            SendReceive *dataSR = createSendReceive();

            Trame *trameToSend = malloc(sizeof(Trame));

            char data[33] = "01101000011000010110110101111010\0";
            // char *data = "01101000011000010110110101111010";

            addMessage(data, trameToSend);
            sendTrame(trameToSend, dataSR);
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
    strncpy(trameToSend->Adr, "11000000\0", 8 * sizeof(char)); // commande de l'ETCD vers l'ETTD : @A = 11000000
}

void sendTrame(Trame *trameToSend, SendReceive *number)
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
    // if (number != NULL)
    // {
    //     number->NS = number->NS + 1;
    // }
}

Trame *receiveTrame(SendReceive *number)
{
    TrameAll *Read = malloc(sizeof(TrameAll));
    read(PIPEDES[0], Read, sizeof(TrameAll));
    Read->trame = *Decode(&(Read->trame));
    if (DEBUG == 1)
    {
        DebugTrameAll(Read, "RECEIVED");
    }
    // if (number != NULL)
    // {
    //     number->NR = number->NR + 1;
    // }
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

SendReceive *createSendReceive()
{
    SendReceive *res = (SendReceive *)malloc(sizeof(SendReceive));
    res->NR = 0;
    res->NS = 0;
    return res;
}

char *createControl(ControlType type, SendReceive *dataSendReceive, int pool, int C1[2], int C2[3])
{

    char Control[9] = "00000000";
    char *NS, *NR;
    switch (type)
    {
    case I:; // Empty Statement
        // UpdateNS(dataSendReceive);
        // UpdateNR(dataSendReceive);
        NS = intToBinaryCharPadded(dataSendReceive->NS, 3);
        NR = intToBinaryCharPadded(dataSendReceive->NR, 3);
        sprintf(Control, "0%s%d%s\0", NS, pool, NR);

        break;
    case S:; // Empty Statement
        if (C1 == NULL)
        {
            printf("\n**Error NULL data (C1)**\n");
            exit(1);
        }
        NR = intToBinaryCharPadded(dataSendReceive->NR, 3);
        sprintf(Control, "10%d%d%d%s\0", C1[0], C1[1], pool, NR);
        break;
    case U:; // Empty Statement
        if (C1 == NULL || C2 == NULL)
        {
            printf("\n**Error NULL data (C1 or C2)**\n");
            exit(1);
        }
        sprintf(Control, "10%d%d%d%d%d%d\0", C1[0], C1[1], pool, C2[0], C2[1], C2[2]);

    default:
        printf("\n**Error while creating Controle**\n");
        break;
    }
}

void UpdateNS(SendReceive *dataSendReceive)
{
    if (dataSendReceive != NULL)
    {
        dataSendReceive->NS = dataSendReceive->NS + 1;
    }
}

void UpdateNR(SendReceive *dataSendReceive)
{
    if (dataSendReceive != NULL)
    {
        dataSendReceive->NS = dataSendReceive->NS + 1;
    }
}