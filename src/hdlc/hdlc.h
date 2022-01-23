#ifndef HDLCINC
#define HDLCINC
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "../crc/crc.h"
#include "../utils/utils.h"


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
void createTrame(char *data, Trame *trameToSend);
void DebugTrameAll(TrameAll *trameA, char *str);
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


#endif