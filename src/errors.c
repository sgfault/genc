#include "../include/errors.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

void add_error(ErrorCollector* collector, ParserErrorType type, const char* message, const char* context)
{
    if (collector->count >= MAX_PARSE_ERRORS)
        return;

    ParseError* error = &collector->errors[collector->count++];
    error->type = type;
    error->message = (char*)message;
    error->context = (char*)context;
}

void report_and_exit(ErrorCollector* collector)
{
    fprintf(stderr, "\nFound %zu errors: \n\n", collector->count);
    for (size_t i = 0; i < collector->count; i++)
    {
        ParseError* error = &collector->errors[i];

        fprintf(stderr, " Error: %s", error->message);
        if (error->context && error->context[0] != '\0')
            fprintf(stderr, " '%s'", error->context);
        fprintf(stderr, "\n");

        if (error->type == ERROR_UNKNOWN_FLAG)
            fprintf(stderr, "     Use 'genc --help' to see available flags\n");
        else if (error->type == ERROR_UNKNOWN_COMMAND)
            fprintf(stderr, "     Use 'genc --help' to see available commands\n");
        else if (error->type == ERROR_MISSING_REQUIRED_ARG)
            fprintf(stderr, "     Use 'genc new <project name>'\n");
    }
    fprintf(stderr, "\n");
    exit(1);
}
