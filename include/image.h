#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>  

unsigned char* readFileToArray(const char* filename, size_t* length);
void createImageFromArray(const unsigned char* data, size_t length, const char* outputFilename);

#endif
