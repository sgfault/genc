#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/command_line_parser.h"
#include "../include/command_new.h"

int main(int argc, char* argv[])
{
    ParseResult parsing_result = parse_command_line(argc, argv);
    if (parsing_result.tag == RESULT_ERR)
        report_and_exit(&parsing_result.data.errors);

    switch (parsing_result.data.command.type)
    {
    case COMMAND_TYPE_BUILD:
        printf("Build command passed\n");
        // init_project(errors, parsing_result.data.command.positional_params.args) break;
        break;
    case COMMAND_TYPE_INIT:
        break;
    case COMMAND_TYPE_NEW:

        break;
    case COMMAND_TYPE_HELP:
        break;
    case COMMAND_TYPE_UNKNOWN:
        break;
    }

    printf("i think it's wokring mf \n");

    return 0;
}
