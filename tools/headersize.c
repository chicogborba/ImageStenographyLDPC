#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1) // Alinha a estrutura para 1 byte

typedef struct
{
    uint16_t bfType;      // Tipo de Arquivo (BM)
    uint32_t bfSize;      // Tamanho do Arquivo
    uint16_t bfReserved1; // Reservado
    uint16_t bfReserved2; // Reservado
    uint32_t bfOffBits;   // Offset dos Dados dos Pixels
} BMPFileHeader;

typedef struct
{
    uint32_t biSize;         // Tamanho do Cabeçalho DIB
    int32_t biWidth;         // Largura da Imagem
    int32_t biHeight;        // Altura da Imagem
    uint16_t biPlanes;       // Número de Planos
    uint16_t biBitCount;     // Bits por Pixel
    uint32_t biCompression;  // Tipo de Compressão
    uint32_t biSizeImage;    // Tamanho dos Dados da Imagem
    int32_t biXPixPerMeter;  // Resolução Horizontal
    int32_t biYPixPerMeter;  // Resolução Vertical
    uint32_t biClrUsed;      // Número de Cores na Paleta
    uint32_t biClrImportant; // Número de Cores Importantes
} BMPInfoHeader;

#pragma pack(pop) // Restaura o alinhamento da estrutura

void readBMPHeader(const char *filename, size_t *headerSize, size_t *pixelDataOffset)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    BMPFileHeader fileHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, file);

    if (fileHeader.bfType != 0x4D42)
    { // Verifica se é um arquivo BMP
        fprintf(stderr, "Not a BMP file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    BMPInfoHeader infoHeader;
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    *headerSize = fileHeader.bfOffBits; // Tamanho do cabeçalho é o offset dos dados dos pixels
    *pixelDataOffset = fileHeader.bfOffBits;

    fclose(file);
}

int main()
{
    const char *filename = "./images/img.bmp";
    size_t headerSize;
    size_t pixelDataOffset;

    readBMPHeader(filename, &headerSize, &pixelDataOffset);

    printf("Header Size: %zu bytes\n", headerSize);
    printf("Pixel Data Offset: %zu bytes\n", pixelDataOffset);

    return 0;
}
