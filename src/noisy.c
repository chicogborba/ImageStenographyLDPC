#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h> 
#include "noisy.h"
#include "image.h"

unsigned char flipBit(unsigned char byte, int position) {
    return byte ^ (1 << position);
}

void noisyChannel(int chanceToFlip, unsigned char* data, size_t length) {
    srand(time(NULL)); 

    unsigned char* pixelData = data + HEADERSIZE;
    size_t pixelDataLength = length - HEADERSIZE;

    for (size_t i = 0; i < pixelDataLength; i++) {
        for (int bit = 0; bit < 8; bit++) {
            if (rand() % 100 < chanceToFlip) {
                pixelData[i] = flipBit(pixelData[i], bit);
            }
        }
    }
}


void printBinary(unsigned char* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (data[i] >> bit) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

double calculateDataLoss(unsigned char* original, unsigned char* noisy, size_t length) {
    int totalBits = length * 8;
    int changedBits = 0;

    for (size_t i = 0; i < length; i++) {
        for (int bit = 0; bit < 8; bit++) {
            if (((original[i] >> bit) & 1) != ((noisy[i] >> bit) & 1)) {
                changedBits++;
            }
        }
    }

    return ((double)changedBits / totalBits) * 100;
}