#include "crc.h"

/**
 * @brief function to compute CRC and codeword
 * 
 * @param dataword 
 * @param generator 
 * @param lenData 
 * @param lenGene 
 * @param lenPad 
 * @return char* 
 */
char *CRC2(char dataword[], char generator[], int lenData, int lenGene, int lenPad)
{
    int l_gen = lenGene;
    int l_data = lenData;
    int gen = toDec(generator, lenGene);

    int dword = toDec(dataword, l_data);

    int dividend = dword << (l_gen - 1);

    int shft = (int)ceill(log2l(dividend + 1)) - l_gen;
    int rem;

    while ((dividend >= gen) || (shft >= 0))
    {
        rem = (dividend >> shft) ^ gen;
        dividend = (dividend & ((1 << shft) - 1)) | (rem << shft);

        shft = (int)ceill(log2l(dividend + 1)) - l_gen;
    }
    return intToBinaryCharPadded(dividend,lenPad);
}



