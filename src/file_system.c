#include "../include/file_system.h"
#include "../include/errors.h"
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

void create_dir(ErrorCollector errors, const char* path)
{

    if (mkdir(path, 0755) && errno != EEXIST)
    {
        // fprintf(stderr, "Failed to create directory %s: %s\n", path, strerror(errno));
        add_error(&errors, FS_ERR_CANNOT_CREATE_DIR, "Failed to create directory", path);
        report_and_exit(&errors);
    }
}

void write_to_file(ErrorCollector errors, const char* path, const char* content)
{
    FILE* f = fopen(path, "w");
    if (!f)
    {
        add_error(&errors, FS_ERR_CANNOT_OPEN_FILE, "Failed to open file", path);
        report_and_exit(&errors);
    }
    if (fputs(content, f) != 0)
    {
        add_error(&errors, FS_ERR_CANNOT_WRITE_TO_FILE, "Failed to write to file", path);
        fclose(f);
        report_and_exit(&errors);
    }
    fclose(f);
}

void read_file(ErrorCollector errors, const char* path)
{
    FILE* fd = fopen(path, "r");
    if (!fd)
    {
        add_error(&errors, FS_ERR_CANNOT_READ_FILE, "Failed to read file", path);
        report_and_exit(&errors);
    }
}

// just create file if does not exist, if exists ignore it
void create_file(ErrorCollector errors, const char* path)
{
}

// walk directories recusively...
void walk_dir(ErrorCollector errors, char* path);
