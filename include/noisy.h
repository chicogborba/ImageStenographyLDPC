#ifndef NOISY_H
#define NOISY_H

#include <stddef.h>

void noisyChannel(int chanceToFlip, unsigned char* data, size_t length);
void printBinary(unsigned char* data, size_t length);
double calculateDataLoss(unsigned char* original, unsigned char* noisy, size_t length);

#endif
