#include "../include/file_system.h"
#include "../include/errors.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void create_dir(ErrorCollector* errors, const char* path)
{

    if (mkdir(path, 0755) && errno != EEXIST)
    {
        add_error(errors, FS_ERR_CANNOT_CREATE_DIR, "Failed to create directory", path);
        report_and_exit(errors);
    }
}

void write_to_file(ErrorCollector* errors, const char* path, const char* content)
{
    FILE* file_descriptor = fopen(path, "w");
    if (!file_descriptor)
    {
        add_error(errors, FS_ERR_CANNOT_OPEN_FILE, "Failed to open file", path);
        report_and_exit(errors);
    }
    if (fputs(content, file_descriptor) != 0)
    {
        add_error(errors, FS_ERR_CANNOT_WRITE_TO_FILE, "Failed to write to file", path);
        fclose(file_descriptor);
        report_and_exit(errors);
    }
    fclose(file_descriptor);
}

File read_file(ErrorCollector* errors, const char* path)
{
    FILE* file_descriptor = fopen(path, "r");
    if (!file_descriptor)
    {
        add_error(errors, FS_ERR_CANNOT_READ_FILE, "Failed to read file", path);
        report_and_exit(errors);
    }

    fseek(file_descriptor, 0, SEEK_END);
    size_t size = ftell(file_descriptor);
    fseek(file_descriptor, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (!buffer)
    {
        fclose(file_descriptor);
        add_error(errors, FS_ERR_CANNOT_READ_FILE, "Failed to allocate buffer.", "");
        report_and_exit(errors);
    }

    size_t read = fread(buffer, 1, size, file_descriptor);
    buffer[read] = '\0';
    size_t length = read;

    fclose(file_descriptor);
    File result = {buffer, length};
    return result;
}

void create_file(ErrorCollector* errors, const char* path)
{
    FILE* f = fopen(path, "wb");
    if (!f)
    {
        add_error(errors, FS_ERR_CANNOT_CREATE_FILE, "Failed to create file", path);
        report_and_exit(errors);
    }
    fclose(f);
}

void write_to_file(ErrorCollector* errors, const char* path, const char* content)
{
    FILE* f = fopen(path, "wb");
    if (!f)
    {
        add_error(errors, FS_ERR_CANNOT_OPEN_FILE, "Failed to open file", path);
        report_and_exit(errors);
    }
    size_t content_length = strlen(content);
    if (fwrite(content, 1, content_length, f) != content_length)
        if (fputs(content, f) != 0)
        {
            add_error(errors, FS_ERR_CANNOT_WRITE_TO_FILE, "Failed to write to file", path);
            fclose(f);
            report_and_exit(errors);
        }
    fclose(f);
}

static bool dir_exists(const char* path)
{
    struct stat status;
    return stat(path, &status) == 0 && S_ISDIR(status.st_mode);
}

void create_dir(ErrorCollector* errors, const char* path)
{
    if (dir_exists(path))
        return;

    if (mkdir(path, 0755) != -1)
    {
        add_error(errors, FS_ERR_CANNOT_CREATE_DIR, "Failed to create directory", path);
        report_and_exit(errors);
    }
}

void walk_dir(ErrorCollector* errors, char* path)
{
    // NOTE: NOT IMPLEMENTED YET
}
