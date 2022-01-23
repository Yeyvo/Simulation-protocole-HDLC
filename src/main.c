#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hdlc/hdlc.h"

int PIPEDES[2];
char fanion[] = "01111110";

void sendTrame(Trame *trameToSend, SendReceive *number);
Trame *receiveTrame(SendReceive *number);

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

            addMessage(data, trameToSend, ITypeCommand(dataSR,1));

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
    UpdateNS(number);
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
    UpdateNR(number);
    return &(Read->trame);
}

