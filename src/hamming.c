/***************************************************************************
 *               Hamming Encoding and Decoding Implementation
 *
 *   File    : hamming.c
 *   Purpose : Provides implementations for Hamming encoding and decoding
 *             routines.
 *   Author  : Michael Dipperstein & Francisco Borba
 *   Date    : July 28, 2024
 *
 ****************************************************************************
 *   Some of these methods are sourced from Michael Dipperstein's repository
 *   on GitHub. For more details and explanations, please refer to the README
 *   file and the accompanying website with detailed documentation.
 ***************************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "hamming.h"

const unsigned char g[DATA_BITS] =
    {
        0x38, /* 0 1 1 | 1 0 0 0 */
        0x54, /* 1 0 1 | 0 1 0 0 */
        0x62, /* 1 1 0 | 0 0 1 0 */
        0x71  /* 1 1 1 | 0 0 0 1 */
};

const unsigned char gT[CODE_BITS] =
    {
        0x07, /* 0 1 1 1 */
        0x0B, /* 1 0 1 1 */
        0x0D, /* 1 1 0 1 */
              /* ------- */
        0x08, /* 1 0 0 0 */
        0x04, /* 0 1 0 0 */
        0x02, /* 0 0 1 0 */
        0x01  /* 0 0 0 1 */
};

const unsigned char h[PARITY_BITS] =
    {
        0x47, /* 1 0 0 | 0 1 1 1 */
        0x2B, /* 0 1 0 | 1 0 1 1 */
        0x1D  /* 0 0 1 | 1 1 0 1 */
};

const unsigned char syndromeMask[PARITY_VALUES] =
    {
        0x00, /* syndrome = 0 0 0 */
        0x10, /* syndrome = 0 0 1 */
        0x20, /* syndrome = 0 1 0 */
        0x08, /* syndrome = 0 1 1 */
        0x40, /* syndrome = 1 0 0 */
        0x04, /* syndrome = 1 0 1 */
        0x02, /* syndrome = 1 1 0 */
        0x01  /* syndrome = 1 1 1 */
};

/***************************************************************************
 *   Function   : SumBitsModulo2
 *   Description: This function performs a modulo 2 sum of the least
 *                significant CODE_BITS bits in an unsigned char.
 *   Parameters : bits - right justified array of CODE_BITS bits to be
 *                       summed modulo 2.
 *   Effects    : None
 *   Returned   : 0 if even number of bits set to 1.
 *                1 if odd number of bits set to 1.
 ***************************************************************************/
unsigned char SumBitsModulo2(unsigned char bits)
{
  unsigned char sum, mask;

  sum = 0;

  for (mask = 0x01; mask < (0x01 << CODE_BITS); mask <<= 1)
  {
    if (bits & mask)
    {
      sum++;
    }
  }

  return (sum & 0x01);
}

/***************************************************************************
 *   Function   : HammingMatrixEncode
 *   Description: This function uses a generator matrix G (above) to
 *                determine the Hamming code for a DATA_BITS long value.
 *                Actually, G Transpose is used in the calculation, because
 *                it makes the C code a little easier.
 *                Notice that '&' is used for modulo 2 multiplication.
 *   Parameters : data - DATA_BITS long series of data bits to encode.
 *   Effects    : None
 *   Returned   : Hamming code used to represent data
 ***************************************************************************/
unsigned char HammingMatrixEncode(unsigned char data)
{
  unsigned char i, code;
  unsigned char rowBits;

  code = 0;

  for (i = 0; i < CODE_BITS; i++)
  {
    code <<= 1; /* code bits are accumulated from right to left */

    /* each component of data row times column c[i] */
    rowBits = (gT[i] & data); /* mod 2 multiplication of vectors */

    /* determine modulo 2 sum of components */
    if (SumBitsModulo2(rowBits))
    {
      code |= 1;
    }
  }

  return code;
}
// TODO: estudar melhor o processo de decode
/***************************************************************************
 *   Function   : HammingMatrixDecode
 *   Description: This function uses the matrix H (above) to determine the
 *                value encoded by a CODE_BITS long code.  H is a parity
 *                check matrix based on the encoding matrix G.  The result
 *                of multiplying the code by H is called the syndrome.  If
 *                there are no errors in the code, the syndrome will be a 0
 *                vector.  If the syndrome is not 0, it will match a column
 *                in H.  The column it matches is likely the errored bit.
 *                Toggle the errored bit and the resulting code is the
 *                nearest matching correct code.
 *   Parameters : code - CODE_BITS long series of code bits to decode.
 *   Effects    : None
 *   Returned   : Nearest value to encoded data
 ***************************************************************************/
unsigned char HammingMatrixDecode(unsigned char code)
{
  unsigned char i, syndromeVal;
  unsigned char syndromeColBits;

  syndromeVal = 0;

  for (i = 0; i < PARITY_BITS; i++)
  {
    syndromeVal <<= 1;

    syndromeColBits = (h[i] & code);

    if (SumBitsModulo2(syndromeColBits))
    {
      syndromeVal |= 1;
    }
  }

  return ((code ^ syndromeMask[syndromeVal]) & (0xFF >> DATA_BITS));
}

/***************************************************************************
 *   Function   : EncodeData
 *   Description: Encodes an array of data bytes using Hamming encoding.
 *   Parameters : data - Pointer to the array of data bytes to encode.
 *                length - Number of bytes in the array.
 *   Effects    : None
 *   Returned   : Pointer to the array of encoded bytes (caller must free it).
 ***************************************************************************/
unsigned char *EncodeData(const unsigned char *data, size_t length)
{
  // Cada byte de 8 bits será codificado em dois bytes de 7 bits, portanto o tamanho será o dobro
  unsigned char *encodedData = (unsigned char *)malloc(length * 2 * sizeof(unsigned char));
  if (encodedData == NULL)
  {
    perror("Error allocating memory for encoded data");
    exit(1);
  }

  size_t j = 0; // Índice para o array codificado
  for (size_t i = 0; i < length; i++)
  {
    unsigned char byte = data[i];
    unsigned char highNibble = (byte >> 4) & 0x0F; // 4 bits mais significativos
    unsigned char lowNibble = byte & 0x0F;         // 4 bits menos significativos

    // Codificar o nibble alto
    encodedData[j++] = HammingMatrixEncode(highNibble);
    // Codificar o nibble baixo
    encodedData[j++] = HammingMatrixEncode(lowNibble);
  }

  return encodedData;
}

/***************************************************************************
 *   Function   : DecodeData
 *   Description: Decodes an array of encoded bytes using Hamming decoding.
 *   Parameters : data - Pointer to the array of encoded bytes to decode.
 *                length - Number of bytes in the array.
 *   Effects    : None
 *   Returned   : Pointer to the array of decoded bytes (caller must free it).
 ***************************************************************************/
unsigned char *DecodeData(const unsigned char *data, size_t length)
{
  // O comprimento original dos dados será a metade do comprimento codificado
  unsigned char *decodedData = (unsigned char *)malloc(length / 2 * sizeof(unsigned char));
  if (decodedData == NULL)
  {
    perror("Error allocating memory for decoded data");
    exit(1);
  }

  size_t j = 0; // Índice para o array decodificado
  for (size_t i = 0; i < length; i += 2)
  {
    // Decodificar dois bytes codificados
    unsigned char highNibble = HammingMatrixDecode(data[i]);
    unsigned char lowNibble = HammingMatrixDecode(data[i + 1]);

    // Reconstituir o byte original
    decodedData[j++] = (highNibble << 4) | (lowNibble & 0x0F);
  }

  return decodedData;
}
