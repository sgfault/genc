#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/command_line_parser.h"

void read_file(ErrorCollector errors, const char* path)
{
    FILE* f = fopen(path, "r");
    if (!f)
    {
        add_error(&errors, FS_ERR_CANNOT_READ_FILE, "Failed to read file", path);
        report_and_exit(&errors);
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (!buffer)
    {
        fclose(f);
        add_error(&errors, FS_ERR_CANNOT_READ_FILE, "Failed to allocate buffer.", "");
    }
}

int main(int argc, char* argv[])
{
    // ParseResult parsing_result = parse_command_line(argc, argv);
    //
    // if (parsing_result.tag == RESULT_ERR)
    //     report_and_exit(&parsing_result.data.errors);

    ErrorCollector err_collector = {0};
    read_file(err_collector, "init.txt");

    printf("i think it's wokring mf \n");

    return 0;
}
