#define _GNU_SOURCE
#include "../include/file_system.h"
#include "../include/errors.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

bool file_exists(const char* path)
{
    struct stat status;
    return stat(path, &status) == 0 && S_ISREG(status.st_mode);
}

char* get_current_dirname(void)
{
    char* full_path = get_current_dir_name();
    if (!full_path)
        return NULL;

    char* dir_name = strrchr(full_path, '/');
    char* result;

    if (dir_name)
        result = strdup(dir_name + 1);
    else
        result = strdup(full_path);

    free(full_path);
    return result;
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

    if (mkdir(path, 0755) != 0)
    {
        add_error(errors, FS_ERR_CANNOT_CREATE_DIR, "Failed to create directory", path);
        report_and_exit(errors);
    }
}

/**
 * @Author: sidati NOUHI
 * @Date: 12/01/2026
 * @Does: Replaces all occurrences of placeholder -> of the form: {{placeholder}}. with actual attribute
 */
char* replace_placeholder(const char* template, const char* attribute, const char* placeholder)
{
    size_t tlen = strlen(template);
    size_t plen = strlen(placeholder);
    size_t nlen = strlen(attribute);

    // Estimate max size
    size_t count = 0;
    for (const char* p = template; (p = strstr(p, placeholder)); p += plen)
        count++;
    size_t newlen = tlen + count * (nlen - plen) + 1;

    char* result = malloc(newlen);
    if (!result)
        exit(1);

    const char* src = template;
    char*       dst = result;
    while ((src = strstr(src, placeholder)))
    {
        size_t prefix = src - template;
        strncpy(dst, template, prefix);
        dst += prefix;
        strcpy(dst, attribute);
        dst += nlen;
        src += plen;
        template = src;
    }
    strcpy(dst, template);
    return result;
}

void walk_dir(ErrorCollector* errors, char* path)
{
    // NOTE: NOT IMPLEMENTED YET
}
