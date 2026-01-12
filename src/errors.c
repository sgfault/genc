#define _POSIX_C_SOURCE 200809L
#include "../include/errors.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_error(ErrorCollector* collector, ErrorType type, const char* message, const char* context)
{
    if (collector->count >= MAX_ERRORS)
        return;
    Error* error = &collector->errors[collector->count++];
    error->type = type;
    error->message = strdup(message);
    error->context = context ? strdup(context) : NULL;
}

void report_and_exit(ErrorCollector* collector)
{
    for (size_t i = 0; i < collector->count; ++i)
    {
        Error* error = &collector->errors[i];
        fprintf(stderr, "[ERROR] %s: %s\n", error->message, error->context ? error->context : "");
    }
    exit(1);
}
