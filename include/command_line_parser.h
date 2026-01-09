#ifndef GENC_COMMAND_LINE_PARSER_H
#define GENC_COMMAND_LINE_PARSER_H

#include "common.h"
#include <stdbool.h>

#define POSITIONAL_ARGS_COUNT 10

typedef struct
{
    bool verbose;
    bool release;
    bool debug;
} CommandFlags;

typedef struct
{
    char*  args[POSITIONAL_ARGS_COUNT];
    size_t length;
} PositionalParams;

typedef struct
{
    CommandType      type;
    PositionalParams positional_params;
    CommandFlags     flags;
} ParsedCommand;

ParsedCommand parse_command_line(int argc, char* argv[]);

#endif  // !GENC_COMMAND_LINE_PARSER_H
