#include <criterion/criterion.h>

#include "../include/command_line_parser.h"

Test(args_parser_command, should_resolve_new_command)
{
    int   argc = 2;
    char* argv[] = {"genc", "new"};

    ParseResult   result = parse_command_line(argc, argv);
    ParsedCommand command = result.data.command;

    cr_assert_eq(command.type, COMMAND_TYPE_NEW);
    cr_assert_eq(command.flags.verbose, false);
    cr_assert_eq(command.flags.release, false);
    cr_assert_eq(command.flags.debug, false);
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
