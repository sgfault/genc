#ifndef GENC_FILE_SYSTEM_H
#define GENC_FILE_SYSTEM_H

#include "../include/errors.h"
#include <stdbool.h>

/* Represents a file read into memory */
typedef struct
{
    char*  buffer;
    size_t length;
} File;

/**
 * Read entire file into memory
 * @param errors: error collector
 * @param path: path to file
 * @return File struct with buffer and length
 */
File read_file(ErrorCollector* errors, const char* path);

/**
 * Create an empty file
 * @param errors: error collector
 * @param path: path to file to create
 */
void create_file(ErrorCollector* errors, const char* path);

/**
 * Write content to a file (overwrites if exists)
 * @param errors: error collector
 * @param path: path to file
 * @param content: content to write
 */
void write_to_file(ErrorCollector* errors, const char* path, const char* content);

/**
 * Check if a file exists
 * @param path: path to file
 * @return true if file exists, false otherwise
 */
bool file_exists(const char* path);

/**
 * Get the basename (directory name) of the current working directory
 * @return newly allocated string with the basename (caller must free), NULL on error
 */
char* get_current_dirname(void);

/**
 * Replace all occurrences of placeholder with attribute in template
 * @param template: template string
 * @param attribute: value to replace placeholder with
 * @param placeholder: placeholder to search for (e.g., "{{project_name}}")
 * @return newly allocated string with replacements (caller must free)
 */
char* replace_placeholder(const char* _template, const char* attribute, const char* placeholder);

/**
 * Create a directory (does nothing if already exists)
 * @param errors: error collector
 * @param name: directory path to create
 */
void create_dir(ErrorCollector* errors, const char* name);

/**
 * Walk through a directory recursively
 * NOTE: Not implemented yet
 * we will use this to execute a call back for recursively on each entry we found
 *
 * @param errors: error collector
 * @param path: directory path to walk
 */
void walk_dir(ErrorCollector* errors, char* path);

#endif  // !GENC_FILE_SYSTEM_H
