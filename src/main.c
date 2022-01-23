#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hdlc/hdlc.h"

int PIPEDES_P[2]; // comunication with parent
int PIPEDES_C[2]; // communication with child
char fanion[] = "01111110";
int mutexPrint = -1;
char PolyGenerator[] = "10001000000100001";
int isChildConnected = FALSE;
int isFatherConnected = FALSE;

Trame *receiveTrame(SendReceive *number, int where);
void sendTrame(Trame *trameToSend, SendReceive *number, int where);

/**
 * @brief Le processus fils contacte le processus pére en utlisant le protocole HDLC
 * le fils : transmet un message sous forme de trame (il doit la creer)
 * le pere : reçois via les pipes la trame HDLC et la traite pour detecter d'eventuelles erreurs
 *
 */
int main(int argc, char const *argv[])
{
    mutexPrint = semget(IPC_PRIVATE, 1, IPC_EXCL | IPC_CREAT | 666);
    if (mutexPrint == -1)
    {
        printf("\n**Erreur Semaphore**\n");
        exit(1);
    }
    semctl(mutexPrint, 0, SETVAL, 1);

    int resPipeP = pipe(PIPEDES_P);
    int resPipeC = pipe(PIPEDES_C);
    if (!resPipeP && !resPipeC)
    {
        int resFork = fork();
        if (resFork > 0)
        {
            // Pere

            SendReceive *dataSR;
            Trame *trameRead;

            while (TRUE)
            {
                if (isFatherConnected == FALSE)
                {
                    dataSR = createSendReceive();
                    trameRead = receiveTrame(dataSR, 0);
                    if (isSABM(trameRead))
                    {
                        isFatherConnected = TRUE;
                    }
                }
                else
                {
                    trameRead = receiveTrame(dataSR, 0);
                    if (isDISC(trameRead))
                    {
                        break;
                    }
                }
            }
        }
        else if (resFork == 0)
        {
            if (DEBUG == 1)
            {
                printf("Press Any Key to Continue\n");
                getchar();
            }
            // fils

            int i = 0;
            SendReceive *dataSR;
            Trame *trameToSend;
            char data[4][33] = {"01101000011000010110110101111010", "10100101011001010100100110101001",
                                "00101010101010000101010010101001", "10101010101010101010101010101001"};
            while (TRUE)
            {
                if (isChildConnected == FALSE)
                {
                    dataSR = createSendReceive();
                    trameToSend = createTrame("", SABMCommand(dataSR, 1));

                    sendTrame(trameToSend, dataSR, 0);
                }
                else
                {
                    if (i <= 3)
                    {
                        trameToSend = createTrame(data[i], ITypeCommand(dataSR, 1));

                        sendTrame(trameToSend, dataSR, 0);
                        i++;
                    }
                    else
                    {
                        printf("\n#####  End Of Connection   #####\n");
                        // disconection
                        trameToSend = createTrame("", DISCCommand(dataSR, 1));
                        sendTrame(trameToSend, dataSR, 0);
                        if (!isChildConnected)
                        {
                            break;
                        }
                    }
                }
            }
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
/**
 * @brief
 *
 * @param trameToSend
 * @param number
 * @param where (0 => write to Parent | 1 => write to child)
 */
void sendTrame(Trame *trameToSend, SendReceive *number, int where)
{
    trameToSend = Encode(trameToSend);
    TrameAll *Write = malloc(sizeof(TrameAll));
    strncpy(Write->fanionStart, fanion, 8 * sizeof(char));
    Write->trame = *trameToSend;

    if (DEBUG == 1)
    {
        DebugTrameAll(Write, "SENT", mutexPrint, number);
    }
    if (where == 0)
    {
        write(PIPEDES_P[1], Write, sizeof(TrameAll));
    }
    else if (where == 1)
    {
        write(PIPEDES_C[1], Write, sizeof(TrameAll));
    }

    if (!isTrameUType(&(Write->trame))) // U type are unumbered
    {
        UpdateNS(number);
    }

    if (isRequestPool(&(Write->trame)))
    { // wait for acknolegement POOL
        printf("\n wait for pool aknow\n");
        Trame *trameRead;
        if (where == 0)
        {
            trameRead = receiveTrame(number, 1);
        }
        else if (where == 1)
        {
            trameRead = receiveTrame(number, 0);
        }
        if (isUA(trameRead))
        {
            /**
             * @TODO ACCEPT(Continue)(nothing to add)
             *
             */
            if ((isSABM(trameToSend) || isSABME(trameToSend)) && isChildConnected == FALSE)
            {
                isChildConnected = TRUE;
            }

            if (isDISC(trameToSend) && isChildConnected == TRUE)
            {
                isChildConnected = FALSE;
            }
        }
        else
        { /**
           * @brief Cas du rejet (on renvois la trame)
           *
           */
            if (isREJ(trameRead))
            {
                number->NR = number->NR - 1;
                number->NS = number->NS - 1;
                sendTrame(trameToSend, number, where);
            }
        }
    }
}
/**
 * @brief
 *
 * @param number
 * @param where (0 => read from Parent | 1 => read from child)
 * @return Trame*
 */
Trame *receiveTrame(SendReceive *number, int where)
{
    TrameAll *Read = malloc(sizeof(TrameAll));

    if (where == 0)
    {
        read(PIPEDES_P[0], Read, sizeof(TrameAll));
    }
    else if (where == 1)
    {
        read(PIPEDES_C[0], Read, sizeof(TrameAll));
    }

    Read->trame = *Decode(&(Read->trame));

    if (!isTrameUType(&(Read->trame))) // U type are unumbered
    {
        UpdateNR(number);
    }

    if (isRequestPool(&(Read->trame)))
    { // aknowlege POOL
        int pass = 1;
        if (isTrameIType(&(Read->trame)))
        {
            Trame *tr = &(Read->trame);
            char *datatoCRC = calloc(strlen(tr->Data) + strlen(tr->FCS), sizeof(char));
            strcat(datatoCRC, tr->Data);
            strcat(datatoCRC, tr->FCS);

            if (atoi(CRC2(datatoCRC, PolyGenerator, strlen(tr->Data) + strlen(tr->FCS), strlen(PolyGenerator), 16)))
            {
                pass = 1;
            }
            else
            {
                !pass;
            }
        }
        if (pass)
        {
            Trame *ackPool = createTrame("", UACommand(number, 0));
            if (where == 0)
            {
                sendTrame(ackPool, number, 1);
            }
            else if (where == 1)
            {
                sendTrame(ackPool, number, 0);
            }
        }
    }

    return &(Read->trame);
}
