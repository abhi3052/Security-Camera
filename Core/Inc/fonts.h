#ifndef __FONTS_H
#define __FONTS_H

#include <stdint.h>

typedef struct
{
    uint8_t width;
    uint8_t height;
    const uint8_t *data;
} FontDef;

extern FontDef Font5x7;

#endif
