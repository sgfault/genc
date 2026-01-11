#ifndef GENC_ERRORS_H
#define GENC_ERRORS_H

#include "common.h"

// NOTE: This header has been modified to globalize error handling throughout the binary
#define MAX_ERRORS 20

typedef enum
{
    PARSE_ERR_UNKNOWN_FLAG,
    PARSE_ERR_UNKNOWN_COMMAND,
    PARSE_ERR_INVALID_NAME,
    PARSE_ERR_TOO_MANY_ARGS,
    PARSE_ERR_MISSING_REQUIRED_ARG,
    PARSE_ERR_FLAGS_NOT_ALLOWED,

    FS_ERR_CANNOT_CREATE_DIR,
    FS_ERR_CANNOT_WALK_DIR,
    FS_ERR_CANNOT_OPEN_FILE,
    FS_ERR_CANNOT_READ_FILE,
    FS_ERR_CANNOT_WRITE_TO_FILE,
} ErrorType;

typedef struct
{
    ErrorType type;
    char*     message;
    char*     context;
} Error;

typedef struct
{
    Error  errors[MAX_ERRORS];
    size_t count;
} ErrorCollector;

void add_error(ErrorCollector* collector, ErrorType type, const char* message, const char* context);
void report_and_exit(ErrorCollector* collector);

#endif  // !GENC_ERRORS_H
