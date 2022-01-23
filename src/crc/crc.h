#ifndef CRCINC
#define CRCINC
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include "../utils/utils.h"

char *CRC2(char dataword[], char generator[], int lenData, int lenGene, int lenPad);

#endif