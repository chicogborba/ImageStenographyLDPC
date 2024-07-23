#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "noisy.h"
#include "image.h"

#define FILENAME "./images/img.bmp"  
#define CHANCE_TO_FLIP 30

int main() {
    size_t length;

    image* img = readFileToImage(FILENAME, &length);
    if (img == NULL) {
        perror("Error reading file");
        exit(1);
    }

    image* originalImage = imgCopy(img);
    if (!originalImage) {
        perror("Error copying image");
        freeImage(img);
        exit(1);
    }

    unsigned char* imgPixelData = pixelArrayToDataArray(img->RGB_data, (length - HEADERSIZE) / 3);
    if (!imgPixelData) {
        perror("Error allocating memory for image pixel data");
        freeImage(img);
        freeImage(originalImage);
        exit(1);
    }

    noisyChannel(CHANCE_TO_FLIP, imgPixelData, length - HEADERSIZE);

    double dataLossPercentage = calculateDataLoss(
        pixelArrayToDataArray(originalImage->RGB_data, (length - HEADERSIZE) / 3), 
        imgPixelData, length - HEADERSIZE
    );

    pixel* noisyPixels = dataArrayToPixelArray(imgPixelData, length - HEADERSIZE);
    if (!noisyPixels) {
        perror("Error allocating memory for noisy pixels");
        freeImage(img);
        freeImage(originalImage);
        free(imgPixelData);
        exit(1);
    }

    image* noisyImage = newImage(length);
    if (!noisyImage) {
        perror("Error creating noisy image");
        freeImage(img);
        freeImage(originalImage);
        free(imgPixelData);
        free(noisyPixels);
        exit(1);
    }

    memcpy(noisyImage->header, img->header, HEADERSIZE);
    noisyImage->RGB_data = noisyPixels;

    createImage(noisyImage, length, "./images/noisy.bmp");
    createImage(originalImage, length, "./images/not-noisy.bmp");

    printf("Dados perdidos: %.2f%%\n", dataLossPercentage);

    freeImage(img);
    freeImage(originalImage);
    free(imgPixelData);
    free(noisyImage);

    return 0;
}
