
#ifndef _HAMMING_H
#define _HAMMING_H
#define DATA_BITS 4
#define PARITY_BITS 3
#include <stddef.h>

#define CODE_BITS (DATA_BITS + PARITY_BITS)
#define PARITY_VALUES (1 << PARITY_BITS)

unsigned char HammingMatrixEncode(unsigned char data);
unsigned char HammingMatrixDecode(unsigned char code);
unsigned char *EncodeData(const unsigned char *data, size_t length);
unsigned char *DecodeData(const unsigned char *data, size_t length);

#endif