#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

unsigned char* readFileToArray(const char* filename, size_t* length) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* data = (unsigned char*)malloc(*length);
    if (!data) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    fread(data, 1, *length, file);
    fclose(file);
    return data;
}

void createImageFromArray(const unsigned char* data, size_t length, const char* outputFilename) {
    FILE* file = fopen(outputFilename, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fwrite(data, 1, length, file);
    fclose(file);
}
