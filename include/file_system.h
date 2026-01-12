#ifndef GENC_FILE_SYSTEM_H
#define GENC_FILE_SYSTEM_H

#include "../include/errors.h"

typedef struct
{
    char*  buffer;
    size_t length;
} File;

// panic("not implemented yet, go use linux");
File read_file(ErrorCollector* errors, const char* path);
void create_file(ErrorCollector* errors, const char* path);
void write_to_file(ErrorCollector* errors, const char* path, const char* content);
void walk_dir(ErrorCollector* errors, char* path);
void create_dir(ErrorCollector* errors, const char* name);

#endif  // !GENC_FILE_SYSTEM_H
