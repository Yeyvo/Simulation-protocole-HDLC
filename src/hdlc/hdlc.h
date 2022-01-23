#ifndef HDLCINC
#define HDLCINC
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "../crc/crc.h"
#include "../utils/utils.h"
#include "../Semaphore/sem.h"


#define DEBUG 1

typedef struct
{
    char Adr[8];
    char Control[9];
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


void addMessage(char *data, Trame *trameToSend, char * control);
Trame * createTrame(char *data, char * control);
void DebugTrameAll(TrameAll *trameA, char *str, int mutex, SendReceive *sendReceiveData);
void DebugTrame(Trame *trame);
Trame *Decode(Trame *trame);
Trame *Encode(Trame *trame);
SendReceive *createSendReceive();
void UpdateNR(SendReceive *dataSendReceive);
void UpdateNS(SendReceive *dataSendReceive);
char *createControl(ControlType type, SendReceive *dataSendReceive, int pool, int C1[2], int C2[3]);
//I-Type generic command
char* ITypeCommand(SendReceive *dataSendReceive, int pool);
//S-Type COMMANDS
char* RRCommand(SendReceive *dataSendReceive, int pool);
char* RNRCommand(SendReceive *dataSendReceive, int pool);
char* REJCommand(SendReceive *dataSendReceive, int pool);
char* SREJCommand(SendReceive *dataSendReceive, int pool);
//U-Type COMMANDS
char* SABMCommand(SendReceive *dataSendReceive, int pool);
char* SABMECommand(SendReceive *dataSendReceive, int pool);
char* UACommand(SendReceive *dataSendReceive, int pool);
char* DISCCommand(SendReceive *dataSendReceive, int pool);
char* DMCommand(SendReceive *dataSendReceive, int pool);
char* FRMRCommand(SendReceive *dataSendReceive, int pool);

int isTrameIType(Trame *trame);
int isTrameSType(Trame *trame);
int isTrameUType(Trame *trame);

int isRR(Trame *trame);
int isRNR(Trame *trame);
int isREJ(Trame *trame);
int isSREJ(Trame *trame);
int isSABM(Trame *trame);
int isSABME(Trame *trame);
int isUA(Trame *trame);
int isDISC(Trame *trame);
int isDM(Trame *trame);
int isFRMR(Trame *trame);
int isRequestPool(Trame* trame);

int getCharType(Trame * trame);
#endif