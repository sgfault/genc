#ifndef GENC_ARGS_PARSER_H
#define GENC_ARGS_PARSER_H

#include "common.h"

typedef struct
{
    CommandType type;
    char*       extra_info;
    char**      flags;
    // we will add more here
} Argv;

Argv parse_args(int argc, char* argv[]);

#endif  // !GENC_ARGS_PARSER_H
