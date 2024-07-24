#ifndef STENO_H
#define STENO_H

#include <stddef.h>
#include "image.h"

void encodeString(image* img, const char* str);
char* decodeString(const image* img);

#endif