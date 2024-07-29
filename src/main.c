#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "noisy.h"
#include "image.h"
#include "stenography.h"
#include "hamming.h"

#define FILENAME "./images/img.bmp"
#define CHANCE_TO_FLIP 1

const char *ENCODEDSTR = "Um bom texto de exemplo para ser codificado";

int main()
{
    size_t length;
    size_t pixelDataLength;

    image *img = readFileToImage(FILENAME, &length);
    if (img == NULL)
    {
        perror("Error reading file");
        exit(1);
    }

    pixelDataLength = length - HEADERSIZE;

    image *originalImage = imgCopy(img);
    if (!originalImage)
    {
        perror("Error copying image");
        freeImage(img);
        exit(1);
    }

    encodeString(img, ENCODEDSTR);

    unsigned char *imgPixelData = pixelArrayToDataArray(img->RGB_data, (pixelDataLength) / 3);
    if (!imgPixelData)
    {
        perror("Error allocating memory for image pixel data");
        freeImage(img);
        freeImage(originalImage);
        exit(1);
    }

    noisyChannel(CHANCE_TO_FLIP, imgPixelData, pixelDataLength);

    double dataLossPercentage = calculateDataLoss(
        pixelArrayToDataArray(originalImage->RGB_data, (pixelDataLength) / 3),
        imgPixelData, length - HEADERSIZE);

    pixel *noisyPixels = dataArrayToPixelArray(imgPixelData, pixelDataLength);
    if (!noisyPixels)
    {
        perror("Error allocating memory for noisy pixels");
        freeImage(img);
        freeImage(originalImage);
        free(imgPixelData);
        exit(1);
    }

    image *noisyImage = newImage(length);
    if (!noisyImage)
    {
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

    char *decodedStr = decodeString(noisyImage);

    printf("Dados perdidos: %.2f%%\n", dataLossPercentage);
    printf("String original: %s\n", ENCODEDSTR);
    printf("String decodificada: %s\n", decodedStr);

    freeImage(img);
    freeImage(originalImage);
    free(imgPixelData);
    free(noisyImage);

    // TEST hamming code
    const unsigned char testData[] = {'A', 'B', 'C', 'D'}; // Sample data for encoding
    size_t dataLength = sizeof(testData) / sizeof(testData[0]);
    size_t encodedLength = dataLength * 2;

    unsigned char *encodedData = EncodeData(testData, dataLength);
    unsigned char *noisyData = (unsigned char *)malloc(encodedLength * sizeof(unsigned char));
    if (noisyData == NULL)
    {
        perror("Error allocating memory for noisy data");
        free(encodedData);
        exit(1);
    }

    memcpy(noisyData, encodedData, encodedLength);

    noisyChannel(5, noisyData, encodedLength);

    unsigned char *decodedData = DecodeData(noisyData, encodedLength);

    printf("Original Data: ");
    for (size_t i = 0; i < dataLength; i++)
    {
        printf("0x%02X ", testData[i]);
    }
    printf("\n");

    printf("Encoded Data: ");
    for (size_t i = 0; i < encodedLength; i++)
    {
        printf("0x%02X ", encodedData[i]);
    }
    printf("\n");

    printf("Noisy Data: ");
    for (size_t i = 0; i < encodedLength; i++)
    {
        printf("0x%02X ", noisyData[i]);
    }
    printf("\n");

    printf("Decoded Data: ");
    for (size_t i = 0; i < dataLength; i++)
    {
        printf("0x%02X ", decodedData[i]);
    }
    printf("\n");

    return 0;
}
