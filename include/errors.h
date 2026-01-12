#ifndef GENC_ERRORS_H
#define GENC_ERRORS_H

#include "common.h"

#define MAX_ERRORS 20

/* All error types */
typedef enum
{
    // Command line parsing errors
    PARSE_ERR_UNKNOWN_FLAG,
    PARSE_ERR_UNKNOWN_COMMAND,
    PARSE_ERR_INVALID_NAME,
    PARSE_ERR_TOO_MANY_ARGS,
    PARSE_ERR_MISSING_REQUIRED_ARG,
    PARSE_ERR_FLAGS_NOT_ALLOWED,

    // File system operation errors
    FS_ERR_CANNOT_CREATE_DIR,
    FS_ERR_CANNOT_WALK_DIR,
    FS_ERR_CANNOT_OPEN_FILE,
    FS_ERR_CANNOT_READ_FILE,
    FS_ERR_CANNOT_WRITE_TO_FILE,
    FS_ERR_CANNOT_CREATE_FILE,
} ErrorType;

/* Single error with type, message, and optional context */
typedef struct
{
    ErrorType type;     // Type of error
    char*     message;  // What went wrong
    char*     context;  // Additional info (file path, flag name, etc) - Nullable
} Error;

/* Collects multiple errors to report them all at once */
typedef struct
{
    Error  errors[MAX_ERRORS];
    size_t count;
} ErrorCollector;

/**
 * Add an error to the collector
 * @param collector: error collector to add to
 * @param type: type of error
 * @param message: error message
 * @param context: additional context (can be NULL or empty string)
 */
void add_error(ErrorCollector* collector, ErrorType type, const char* message, const char* context);

/**
 * Print all collected errors to stderr and exit with code 1
 * @param collector: error collector with errors to report
 */
void report_and_exit(ErrorCollector* collector);

#endif  // !GENC_ERRORS_H
