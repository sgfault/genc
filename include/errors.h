#ifndef GENC_ERRORS_H
#define GENC_ERRORS_H

#include "common.h"

// NOTE: This header has been modified to globalize error handling throughout the binary
#define MAX_ERRORS 20

typedef enum
{
    ERROR_TYPE_PARSER,
    ERROR_TYPE_FILESYSTEM,
} ErrorType;

typedef enum
{
    ERROR_UNKNOWN_FLAG,
    ERROR_UNKNOWN_COMMAND,
    ERROR_INVALID_NAME,
    ERROR_TOO_MANY_ARGS,
    ERROR_MISSING_REQUIRED_ARG,
    ERROR_FLAGS_NOT_ALLOWED,
} ParserErrorType;

typedef enum
{
    FS_ERROR_CANNOT_CREATE_DIR,
    FS_ERROR_CANNOT_WALK_DIR,
    FS_ERROR_CANNOT_OPEN_FILE,
    FS_ERROR_CANNOT_READ_FILE,
    FS_ERROR_CANNOT_WRITE_TO_FILE,
} FileSystemErrorType;

typedef struct
{
    ErrorType type;
    union
    {
        ParserErrorType     parser;
        FileSystemErrorType fs;
    } subtype;
    char* message;
    char* context;
} Error;

typedef struct
{
    Error  errors[MAX_ERRORS];
    size_t count;
} ErrorCollector;

void add_error(ErrorCollector* collector, ErrorType type, void* subtype, const char* message, const char* context);
void report_and_exit(ErrorCollector* collector);

#endif  // !GENC_ERRORS_H
