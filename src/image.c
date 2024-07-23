#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

image* readFileToImage(const char* filename, size_t* length) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Corrigir alocação do cabeçalho para HEADERSIZE
    unsigned char* header = (unsigned char*)malloc(HEADERSIZE);
    if (!header) {
        perror("Error allocating memory for header");
        fclose(file);
        return NULL;
    }
    fread(header, 1, HEADERSIZE, file);

    // Alocar memória para os dados RGB como bytes
    unsigned char* rgb_data = (unsigned char*)malloc(*length - HEADERSIZE);
    if (!rgb_data) {
        perror("Error allocating memory for RGB data");
        free(header);
        fclose(file);
        return NULL;
    }

    fread(rgb_data, 1, *length - HEADERSIZE, file);
    fclose(file);

    // Converter os dados RGB para a estrutura de pixels
    pixel* pixels = dataArrayToPixelArray(rgb_data, *length - HEADERSIZE);
    free(rgb_data); // Libera a memória temporária dos dados RGB

    image* img = (image*)malloc(sizeof(image));
    if (!img) {
        perror("Error allocating memory for image");
        free(header);
        free(pixels);
        return NULL;
    }

    img->header = header;
    img->RGB_data = pixels;
    img->length = *length;

    return img;
}

void createImage(const image* img, size_t length, const char* outputFilename) {
    FILE* file = fopen(outputFilename, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fwrite(img->header, 1, HEADERSIZE, file);
    unsigned char* rgb_data = pixelArrayToDataArray(img->RGB_data, (length - HEADERSIZE) / sizeof(pixel));
    fwrite(rgb_data, 1, length - HEADERSIZE, file);

    fclose(file);
}

void freeImage(image* img) {
    free(img->header);
    free(img->RGB_data);
    free(img);
}

pixel* dataArrayToPixelArray(unsigned char* data, size_t length) {
    size_t numPixels = length / 3;
    pixel* RGB_data = (pixel*)malloc(numPixels * sizeof(pixel));
    if (!RGB_data) {
        perror("Error allocating memory");
        return NULL;
    }

    for (size_t i = 0; i < numPixels; i++) {
        RGB_data[i].R = data[i * 3];
        RGB_data[i].G = data[i * 3 + 1];
        RGB_data[i].B = data[i * 3 + 2];
    }

    return RGB_data;
}

unsigned char* pixelArrayToDataArray(const pixel* pixels, size_t numPixels) {
    unsigned char* data = (unsigned char*)malloc(numPixels * 3);
    if (!data) {
        perror("Error allocating memory");
        return NULL;
    }

    for (size_t i = 0; i < numPixels; i++) {
        data[i * 3] = pixels[i].R;
        data[i * 3 + 1] = pixels[i].G;
        data[i * 3 + 2] = pixels[i].B;
    }

    return data;
}

image* imgCopy(const image* src) {
    image* dest = (image*)malloc(sizeof(image));
    if (!dest) {
        perror("Error allocating memory for image copy");
        return NULL;
    }

    dest->header = (unsigned char*)malloc(HEADERSIZE);
    if (!dest->header) {
        perror("Error allocating memory for header copy");
        free(dest);
        return NULL;
    }
    memcpy(dest->header, src->header, HEADERSIZE);

    size_t numPixels = (src->length - HEADERSIZE) / 3;
    dest->RGB_data = (pixel*)malloc(numPixels * sizeof(pixel));
    if (!dest->RGB_data) {
        perror("Error allocating memory for RGB data copy");
        free(dest->header);
        free(dest);
        return NULL;
    }
    memcpy(dest->RGB_data, src->RGB_data, numPixels * sizeof(pixel));

    dest->length = src->length;

    return dest;
}


image* newImage(size_t length) {
    image* img = (image*)malloc(sizeof(image));
    if (!img) {
        perror("Error allocating memory for image");
        return NULL;
    }

    img->header = (unsigned char*)malloc(HEADERSIZE);
    if (!img->header) {
        perror("Error allocating memory for header");
        free(img);
        return NULL;
    }

    img->RGB_data = (pixel*)malloc((length - HEADERSIZE) / 3 * sizeof(pixel));
    if (!img->RGB_data) {
        perror("Error allocating memory for RGB data");
        free(img->header);
        free(img);
        return NULL;
    }

    img->length = length;

    return img;
}
