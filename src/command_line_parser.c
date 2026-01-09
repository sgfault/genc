#include "../include/command_line_parser.h"
#include "../include/errors.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static CommandType resolve_command_type(const char* command)
{
    if (!strcmp(command, "init"))
        return COMMAND_TYPE_INIT;
    else if (!strcmp(command, "new"))
        return COMMAND_TYPE_NEW;
    else if (!strcmp(command, "build"))
        return COMMAND_TYPE_BUILD;
    else
        return COMMAND_TYPE_UNKNOWN;
}

static inline bool matches_flag(const char* arg, const char* long_flag, const char* short_flag)
{
    return !strcmp(arg, long_flag) || !strcmp(arg, short_flag);
}

ParseResult parse_command_line(int argc, char* argv[])
{
    ErrorCollector errors = {0};

    if (argc <= 1)
    {
        add_error(&errors, ERROR_MISSING_REQUIRED_ARG, "No command provided.", "");
        report_and_exit(&errors);
    }

    if (matches_flag(argv[1], "--help", "-h"))
    {
        printf("Show help later, for now god help you. :Joy\n");
        exit(0);
    }

    char*       command = argv[1];
    CommandType type = resolve_command_type(command);

    if (type == COMMAND_TYPE_UNKNOWN)
    {
        add_error(&errors, ERROR_UNKNOWN_COMMAND, "Unknown command.", command);
    }

    CommandFlags flags = {false, false, false};
    char*        positional_args_items[POSITIONAL_ARGS_COUNT];
    size_t       positional_args_count = 0;

    int i = 2;
    while (i < argc)
    {
        char* arg = argv[i];

        if ('-' == arg[0])
        {
            if (matches_flag(arg, "--verbose", "-v"))
                flags.verbose = true;
            else if (matches_flag(arg, "--release", "-r"))
                flags.release = true;
            else if (matches_flag(arg, "--debug", "-d"))
                flags.debug = true;
            else
                add_error(&errors, ERROR_UNKNOWN_FLAG, "Unknown flag.", arg);
        }
        else
        {
            if (positional_args_count >= POSITIONAL_ARGS_COUNT)
                add_error(&errors, ERROR_TOO_MANY_ARGS, "too many arguments.", arg);
            else
                positional_args_items[positional_args_count++] = arg;
        }
        i++;
    }

    if (errors.count > 0)
    {
        ParseResult result;
        result.tag = RESULT_ERR;
        result.data.errors = errors;
        return result;
    }

    ParseResult result;
    result.tag = RESULT_OK;
    result.data.command.type = type;
    result.data.command.flags = flags;
    for (size_t i = 0; i < positional_args_count; i++)
    {
        result.data.command.positional_params.args[i] = positional_args_items[i];
    }
    result.data.command.positional_params.length = positional_args_count;

    return result;
}
