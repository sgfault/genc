#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <stdio.h>

#include "../include/command_line_parser.h"

Test(args_parser_command, should_resolve_new_command)
{
    int   argc = 3;
    char* argv[] = {"genc", "new", "project_name"};

    ParseResult   result = parse_command_line(argc, argv);
    ParsedCommand command = result.data.command;

    cr_assert_eq(result.tag, RESULT_OK);
    cr_assert_eq(command.type, COMMAND_TYPE_NEW);
    cr_assert(command.positional_params.length > 0);
    cr_assert_eq(command.positional_params.args[0], "project_name");
    cr_assert_eq(command.flags.verbose, false);
    cr_assert_eq(command.flags.release, false);
    cr_assert_eq(command.flags.debug, false);
}

Test(arg_parses_command, should_report_project_name_not_specified)
{
    int   argc = 2;
    char* argv[] = {"genc", "new"};

    ParseResult    result = parse_command_line(argc, argv);
    ErrorCollector errors = result.data.errors;

    cr_assert_eq(result.tag, RESULT_ERR);
    cr_assert_eq(errors.count, 1);
    cr_assert_str_eq(errors.errors[0].message, "project name not specified");
}

Test(args_parser_command, should_resolve_init_command)
{
    int   argc = 2;
    char* argv[] = {"genc", "init"};

    ParseResult   result = parse_command_line(argc, argv);
    ParsedCommand command = result.data.command;

    cr_assert_eq(result.tag, RESULT_OK);
    cr_assert_eq(command.type, COMMAND_TYPE_INIT);
    cr_assert_eq(command.flags.verbose, false);
    cr_assert_eq(command.flags.release, false);
    cr_assert_eq(command.flags.debug, false);
}

Test(args_parser_command, should_resolve_build_command)
{
    int   argc = 2;
    char* argv[] = {"genc", "build"};

    ParseResult   result = parse_command_line(argc, argv);
    ParsedCommand command = result.data.command;

    cr_assert_eq(result.tag, RESULT_OK);
    cr_assert_eq(command.type, COMMAND_TYPE_BUILD);
    cr_assert_eq(command.flags.verbose, false);
    cr_assert_eq(command.flags.release, false);
    cr_assert_eq(command.flags.debug, false);
}

Test(args_parse_command, should_report_unknown_commands)
{
    int   argc = 2;
    char* argv[] = {"genc", "typobuild"};

    ParseResult    result = parse_command_line(argc, argv);
    ErrorCollector errors = result.data.errors;

    cr_assert_eq(errors.count, 1);
    cr_assert_str_eq(errors.errors[0].message, "Unknown command.");
    cr_assert_str_eq(errors.errors[0].context, "typobuild");
}

Test(args_parser_flags, should_resolve_flags)
{
    int   argc = 5;
    char* argv[] = {"genc", "build", "--release", "-d", "-v"};

    ParseResult   result = parse_command_line(argc, argv);
    ParsedCommand command = result.data.command;

    cr_assert_eq(result.tag, RESULT_OK);
    cr_assert_eq(command.type, COMMAND_TYPE_BUILD);
    cr_assert_eq(command.flags.verbose, true);
    cr_assert_eq(command.flags.release, true);
    cr_assert_eq(command.flags.debug, true);
}

Test(args_parser_flags, should_report_unkown_flags)
{
    int   argc = 5;
    char* argv[] = {"genc", "build", "--release", "-d", "--shit"};

    ParseResult    result = parse_command_line(argc, argv);
    ErrorCollector errors = result.data.errors;

    cr_assert_eq(errors.count, 1);
    cr_assert_str_eq(errors.errors[0].message, "Unknown flag.");
    cr_assert_str_eq(errors.errors[0].context, "--shit");
}

Test(args_parser_flags, should_report_not_allowed_flags)
{
    int   argc = 4;
    char* argv[] = {"genc", "init", "--release", "-v"};

    ParseResult    result = parse_command_line(argc, argv);
    ErrorCollector errors = result.data.errors;

    cr_assert_eq(errors.count, 2);
    cr_assert_str_eq(errors.errors[0].message, "flag not allowed for this command");
    cr_assert_str_eq(errors.errors[0].context, "--release");
    cr_assert_str_eq(errors.errors[1].message, "flag not allowed for this command");
    cr_assert_str_eq(errors.errors[1].context, "-v");
}
