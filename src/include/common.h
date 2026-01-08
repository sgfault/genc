#ifndef GENC_COMMON_H
#define GENC_COMMON_H

#include "command.h"
#include <stddef.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t   size;

typedef struct
{
    char* chars;
    size  length;
} Str;

Str new_string(const char* chars);

#endif  // !GENC_COMMON_H
