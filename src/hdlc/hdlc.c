#include "hdlc.h"

char Generator[] = "10001000000100001";

void addMessage(char *data, Trame *trameToSend, char * control)
{
    /**
     * @brief NEED to handle case where input data is less or more than 32
     *
     */
    // strncpy(trameToSend->Data, data, strlen(data) * sizeof(char));
    strncpy(trameToSend->Data, data, strlen(data) * sizeof(char));
    strncpy(trameToSend->FCS, CRC2(data, Generator, strlen(data), strlen(Generator)), 16 * sizeof(char));
    strncpy(trameToSend->Adr, "11000000", 8 * sizeof(char)); // commande de l'ETCD vers l'ETTD : @A = 11000000
    strncpy(trameToSend->Control, control, 8 * sizeof(char)); 
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

    char *Control = (char *) calloc(9,sizeof(char));
    
    char *NS, *NR;
    switch (type)
    {
    case I:; // Empty Statement
        // UpdateNS(dataSendReceive);
        // UpdateNR(dataSendReceive);
        NS = intToBinaryCharPadded(dataSendReceive->NS, 3);
        NR = intToBinaryCharPadded(dataSendReceive->NR, 3);
        snprintf(Control,9, "0%s%d%s", NS, pool, NR);

        break;
    case S:; // Empty Statement
        if (C1 == NULL)
        {
            printf("\n**Error NULL data (C1)**\n");
            exit(1);
        }
        NR = intToBinaryCharPadded(dataSendReceive->NR, 3);
        snprintf(Control,9, "10%d%d%d%s", C1[0], C1[1], pool, NR);
        break;
    case U:; // Empty Statement
        if (C1 == NULL || C2 == NULL)
        {
            printf("\n**Error NULL data (C1 or C2)**\n");
            exit(1);
        }
        snprintf(Control,9, "10%d%d%d%d%d%d", C1[0], C1[1], pool, C2[0], C2[1], C2[2]);

    default:
        printf("\n**Error while creating Controle**\n");
        exit(1);
        break;
    }
    Control[8] = '\0';
    return Control;
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



//I-Type
char* ITypeCommand(SendReceive *dataSendReceive, int pool){
    return createControl(I,dataSendReceive,pool,NULL,NULL);
}

//S-Type
char* RRCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {0,0};
    return createControl(S,dataSendReceive,pool,C1,NULL);
}

char* RNRCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {1,0};
    return createControl(S,dataSendReceive,pool,C1,NULL);
}

char* REJCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {0,1};
    return createControl(S,dataSendReceive,pool,C1,NULL);
}

char* SREJCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {1,1};
    return createControl(S,dataSendReceive,pool,C1,NULL);
}

//U-Type

char* SABMCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {1,1};
    int C2[3] = {1,0,0};
    return createControl(S,dataSendReceive,pool,C1,C2);
}
char* SABMECommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {1,1};
    int C2[3] = {1,1,0};
    return createControl(S,dataSendReceive,pool,C1,C2);
}
char* UACommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {0,0};
    int C2[3] = {1,1,0};
    return createControl(S,dataSendReceive,pool,C1,C2);
}
char* DISCCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {0,0};
    int C2[3] = {0,1,0};
    return createControl(S,dataSendReceive,pool,C1,C2);
}
char* DMCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {1,1};
    int C2[3] = {0,0,0};
    return createControl(S,dataSendReceive,pool,C1,C2);
}
char* FRMRCommand(SendReceive *dataSendReceive, int pool){
    int C1[2] = {1,0};
    int C2[3] = {0,0,1};
    return createControl(S,dataSendReceive,pool,C1,C2);
}