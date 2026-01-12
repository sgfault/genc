#include "../include/command_line_parser.h"
#include "../include/errors.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    CommandType cmd;
    bool        allow_verbose;
    bool        allow_debug;
    bool        allow_release;
} CommandFlagRules;

static const CommandFlagRules ALLOWED_RULES[] = {
    {COMMAND_TYPE_BUILD, true, true, true},
    {COMMAND_TYPE_INIT, false, false, false},
    {COMMAND_TYPE_NEW, false, false, false},
};

static bool is_flag_allowed(CommandType cmd, const char* flag_name)
{
    for (size_t i = 0; i < sizeof(ALLOWED_RULES) / sizeof(ALLOWED_RULES[0]); i++)
    {
        if (ALLOWED_RULES[i].cmd == cmd)
        {
            if (!strcmp(flag_name, "verbose"))
                return ALLOWED_RULES[i].allow_verbose;
            if (!strcmp(flag_name, "debug"))
                return ALLOWED_RULES[i].allow_debug;
            if (!strcmp(flag_name, "release"))
                return ALLOWED_RULES[i].allow_release;
        }
    }
    return false;
}

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

ParseResult parse_command_line(ErrorCollector* errors, int argc, char* argv[])
{
    if (argc <= 1)
    {
        add_error(errors, PARSE_ERR_MISSING_REQUIRED_ARG, "No command provided.", "");
        report_and_exit(errors);
    }

    if (matches_flag(argv[1], "--help", "-h"))
    {
        ParseResult result;
        result.tag = RESULT_OK;
        result.data.command.type = COMMAND_TYPE_HELP;
        return result;
    }

    char*       command = argv[1];
    CommandType command_type = resolve_command_type(command);

    if (command_type == COMMAND_TYPE_UNKNOWN)
        add_error(errors, PARSE_ERR_UNKNOWN_COMMAND, "Unknown command.", command);

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
            {
                if (!is_flag_allowed(command_type, "verbose"))
                    add_error(errors, PARSE_ERR_FLAGS_NOT_ALLOWED, "flag not allowed for this command", arg);
                else
                    flags.verbose = true;
            }
            else if (matches_flag(arg, "--release", "-r"))
            {
                if (!is_flag_allowed(command_type, "release"))
                {
                    add_error(errors, PARSE_ERR_FLAGS_NOT_ALLOWED, "flag not allowed for this command", arg);
                }
                else
                    flags.release = true;
            }
            else if (matches_flag(arg, "--debug", "-d"))
            {
                if (!is_flag_allowed(command_type, "debug"))
                    add_error(errors, PARSE_ERR_FLAGS_NOT_ALLOWED, "flag not allowed for this command", arg);
                else
                    flags.debug = true;
            }
            else
                add_error(errors, PARSE_ERR_UNKNOWN_FLAG, "Unknown flag.", arg);
        }
        else
        {
            if (positional_args_count >= POSITIONAL_ARGS_COUNT)
                add_error(errors, PARSE_ERR_TOO_MANY_ARGS, "too many arguments.", arg);
            else
                positional_args_items[positional_args_count++] = arg;
        }
        i++;
    }

    // NOTE: here we will add commands specific validations
    if (command_type == COMMAND_TYPE_NEW)
    {
        if (positional_args_count < 1)
            add_error(errors, PARSE_ERR_MISSING_REQUIRED_ARG, "project name not specified", command);
        // TODO: validate project name and so on
    }

    if (errors->count > 0)
    {
        ParseResult result;
        result.tag = RESULT_ERR;
        result.data.errors = errors;
        return result;
    }

    ParseResult result;
    result.tag = RESULT_OK;
    result.data.command.type = command_type;
    result.data.command.flags = flags;
    for (size_t i = 0; i < positional_args_count; i++)
    {
        result.data.command.positional_params.args[i] = positional_args_items[i];
    }
    result.data.command.positional_params.length = positional_args_count;

    return result;
}
