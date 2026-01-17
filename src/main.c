#include "../include/command_init.h"
#include "../include/command_line_parser.h"
#include "../include/command_new.h"
#include "../include/errors.h"

#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

    ParseResult parsing_result = parse_command_line(&collector, argc, argv);
    if (parsing_result.tag == RESULT_ERR)
        report_and_exit(&collector);

    switch (parsing_result.data.command.type)
    {
    case COMMAND_TYPE_NEW:
        create_new_project(&collector, parsing_result.data.command.positional_params.args[0]);
        break;
    case COMMAND_TYPE_BUILD:
        printf("Build command passed\n");
        break;
    case COMMAND_TYPE_INIT:
        init_project(&collector);
        break;
    case COMMAND_TYPE_HELP:
        printf("Help command passed\n");
        break;
    case COMMAND_TYPE_UNKNOWN:
        printf("Unknown command passed\n");
        break;
    }

    return 0;
}
