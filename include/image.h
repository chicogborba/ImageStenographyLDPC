#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>  

#define HEADERSIZE 138

typedef struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} pixel;
typedef struct image {
    pixel* RGB_data;
    unsigned char* header;
    size_t length;
} image;

image* readFileToImage(const char* filename, size_t* length);
void createImage(const image* img, size_t length, const char* outputFilename);
unsigned char* pixelArrayToDataArray(const pixel* pixels, size_t numPixels);
pixel* dataArrayToPixelArray(unsigned char* data, size_t length);
void freeImage(image* img);
image* imgCopy(const image* src);
image* newImage(size_t length);

#endif
