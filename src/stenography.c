#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "image.h"
#include "stenography.h"

void encodeString(image *img, const char *str)
{
    size_t strLength = strlen(str) + 1;
    size_t numPixels = (img->length - HEADERSIZE) / 3;
    if (strLength * 3 > numPixels)
    {
        fprintf(stderr, "Error: Image is too small to encode string\n");
        return;
    }

    for (size_t i = 0, pixelIdx = 0; i < strLength; i++)
    {
        img->RGB_data[pixelIdx].R = (img->RGB_data[pixelIdx].R & 0xFE) | ((str[i] >> 7) & 0x01);
        img->RGB_data[pixelIdx].G = (img->RGB_data[pixelIdx].G & 0xFE) | ((str[i] >> 6) & 0x01);
        img->RGB_data[pixelIdx].B = (img->RGB_data[pixelIdx].B & 0xFE) | ((str[i] >> 5) & 0x01);
        pixelIdx++;

        img->RGB_data[pixelIdx].R = (img->RGB_data[pixelIdx].R & 0xFE) | ((str[i] >> 4) & 0x01);
        img->RGB_data[pixelIdx].G = (img->RGB_data[pixelIdx].G & 0xFE) | ((str[i] >> 3) & 0x01);
        img->RGB_data[pixelIdx].B = (img->RGB_data[pixelIdx].B & 0xFE) | ((str[i] >> 2) & 0x01);
        pixelIdx++;

        img->RGB_data[pixelIdx].R = (img->RGB_data[pixelIdx].R & 0xFE) | ((str[i] >> 1) & 0x01);
        img->RGB_data[pixelIdx].G = (img->RGB_data[pixelIdx].G & 0xFE) | (str[i] & 0x01);
        pixelIdx++;
    }
}

char *decodeString(const image *img)
{
    size_t numPixels = (img->length - HEADERSIZE) / 3;
    char *str = (char *)malloc(numPixels / 3 + 1);
    if (!str)
    {
        perror("Error allocating memory for decoded string");
        return NULL;
    }

    for (size_t i = 0, pixelIdx = 0; i < numPixels / 3; i++)
    {
        str[i] = (img->RGB_data[pixelIdx].R & 0x01) << 7;
        str[i] |= (img->RGB_data[pixelIdx].G & 0x01) << 6;
        str[i] |= (img->RGB_data[pixelIdx].B & 0x01) << 5;
        pixelIdx++;

        str[i] |= (img->RGB_data[pixelIdx].R & 0x01) << 4;
        str[i] |= (img->RGB_data[pixelIdx].G & 0x01) << 3;
        str[i] |= (img->RGB_data[pixelIdx].B & 0x01) << 2;
        pixelIdx++;

        str[i] |= (img->RGB_data[pixelIdx].R & 0x01) << 1;
        str[i] |= img->RGB_data[pixelIdx].G & 0x01;
        pixelIdx++;

        if (str[i] == '\0')
        {
            break;
        }
    }

    return str;
}
