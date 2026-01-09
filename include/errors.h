#ifndef GENC_ERRORS_H
#define GENC_ERRORS_H

#include "common.h"

#define MAX_PARSE_ERRORS 20

typedef enum
{
    ERROR_UNKNOWN_FLAG,
    ERROR_UNKNOWN_COMMAND,
    ERROR_INVALID_NAME,
    ERROR_TOO_MANY_ARGS,
    ERROR_MISSING_REQUIRED_ARG,
} ParserErrorType;

typedef struct
{
    ParserErrorType type;
    char*           message;
    char*           context;
} ParseError;

typedef struct
{
    ParseError errors[MAX_PARSE_ERRORS];
    size_t     count;
} ErrorCollector;

void add_error(ErrorCollector* collector, ParserErrorType type, const char* message, const char* context);
void report_and_exit(ErrorCollector* collector);

#endif  // !GENC_ERRORS_H
