#ifndef GENC_COMMAND_LINE_PARSER_H
#define GENC_COMMAND_LINE_PARSER_H

#include "common.h"
#include "errors.h"
#include <stdbool.h>

#define POSITIONAL_ARGS_COUNT 10

/* Boolean flags that can be passed to commands */
typedef struct
{
    bool verbose;  // -v, --verbose
    bool release;  // -r, --release
    bool debug;    // -d, --debug
} CommandFlags;

/* Positional arguments (eg: <project_name> ...) */
typedef struct
{
    char*  args[POSITIONAL_ARGS_COUNT];
    size_t length;
} PositionalParams;

/* Parsing result in case of success */
typedef struct
{
    CommandType      type;
    PositionalParams positional_params;
    CommandFlags     flags;
} ParsedCommand;

/* Result tag to know if parsing succeeded or failed */
typedef enum
{
    RESULT_OK,
    RESULT_ERR
} ResultTag;

/* Result of parsing - either a valid command or errors
 * Use tag to check which union member is valid */
typedef struct
{
    ResultTag tag;
    union
    {
        ParsedCommand  command;  // Valid when tag == RESULT_OK
        ErrorCollector errors;   // Valid when tag == RESULT_ERR
    } data;
} ParseResult;

/**
 * Parse command line arguments into a structured result
 * @param argc: argument count from main
 * @param argv: argument vector from main
 * @return ParseResult with either parsed command or errors
 */
ParseResult parse_command_line(int argc, char* argv[]);

#endif  // !GENC_COMMAND_LINE_PARSER_H
