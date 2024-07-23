#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "noisy.h"
#include "image.h"

#define FILENAME "./images/img.bmp"  
#define CHANCE_TO_FLIP 5

int main() {
    size_t length;
    unsigned char* data = readFileToArray(FILENAME, &length);
    if (data == NULL) {
        perror("Error reading file");
        exit(1);
    }

    unsigned char* originalData = (unsigned char*)malloc(length);
    if (!originalData) {
        perror("Error allocating memory");
        free(data);
        exit(1);
    }

    memcpy(originalData, data, length);

    noisyChannel(CHANCE_TO_FLIP, data, length);

    double dataLossPercentage = calculateDataLoss(originalData, data, length);
    printf("Dados perdidos: %.2f%%\n", dataLossPercentage);

    createImageFromArray(data, length, "./images/noisy.bmp");
    createImageFromArray(originalData, length, "./images/not-noisy.bmp");

    free(originalData);
    free(data);

    return 0;
}
