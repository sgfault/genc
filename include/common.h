#ifndef GENC_COMMON_H
#define GENC_COMMON_H

#include "command.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct
{
    char*  chars;
    size_t length;
} Str;

Str new_string(const char* chars);

/**
 * Prompt user with a yes/no question
 * @param prompt: the question to ask the user
 * @return true if user answered yes (y/Y), false otherwise
 */
bool prompt_yes_no(const char* prompt);

#endif  // !GENC_COMMON_H
