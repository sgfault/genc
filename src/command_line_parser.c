#include "../include/command_line_parser.h"
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

ParsedCommand parse_command_line(int argc, char* argv[])
{
    if (argc <= 1 || matches_flag(argv[1], "--help", "-h"))
    {
        fprintf(stderr, "you need help may god help you\n");
        exit(65);
    }

    char*       command = argv[1];
    CommandType type = resolve_command_type(command);
    bool        verbose = false, debug = false, release = false;
    char*       positional_args_items[POSITIONAL_ARGS_COUNT];
    size_t      positional_args_count = 0;

    int i = 2;
    while (i < argc)
    {
        char* arg_value = argv[i];

        if ('-' == arg_value[0])
        {
            if (matches_flag(arg_value, "--verbose", "-v"))
                verbose = true;
            else if (matches_flag(arg_value, "--release", "-r"))
                release = true;
            else if (matches_flag(arg_value, "--debug", "-d"))
                debug = true;
        }
        else
        {
            if (positional_args_count + 1 >= POSITIONAL_ARGS_COUNT)
            {
                fprintf(stderr, "too many arguments\n");
                exit(65);
            }
            positional_args_items[positional_args_count++] = arg_value;
        }
        i++;
    }

    ParsedCommand result;
    result.type = type;
    result.flags.verbose = verbose;
    result.flags.release = release;
    result.flags.debug = debug;

    for (size_t i = 0; i < positional_args_count; i++)
    {
        result.positional_params.args[i] = positional_args_items[i];
    }
    result.positional_params.length = positional_args_count;

    return result;
}
