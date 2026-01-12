#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/command_line_parser.h"

int main(int argc, char* argv[])
{
    ParseResult parsing_result = parse_command_line(argc, argv);
    if (parsing_result.tag == RESULT_ERR)
        report_and_exit(&parsing_result.data.errors);

    printf("i think it's wokring mf \n");

    return 0;
}
