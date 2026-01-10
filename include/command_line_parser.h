#ifndef GENC_COMMAND_LINE_PARSER_H
#define GENC_COMMAND_LINE_PARSER_H

#include "common.h"
#include "errors.h"
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

typedef enum
{
    RESULT_OK,
    RESULT_ERR
} ResultTag;

typedef struct
{
    ResultTag tag;
    union
    {
        ParsedCommand  command;
        ErrorCollector errors;
    } data;
} ParseResult;

ParseResult parse_command_line(int argc, char* argv[]);

#endif  // !GENC_COMMAND_LINE_PARSER_H
