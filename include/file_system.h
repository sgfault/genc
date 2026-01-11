#ifndef GENC_FILE_SYSTEM_H
#define GENC_FILE_SYSTEM_H

#include "../include/errors.h"
// panic("not implemented yet, go use linux");

void read_file(ErrorCollector errors, const char* path);

// just create file if does not exist, if exists ignore it
void create_file(ErrorCollector errors, const char* path);

// write to a file, create it if does not exist then write to it
void write_to_file(ErrorCollector errors, const char* path, const char* content);

// walk directories recusive...
void walk_dir(ErrorCollector errors, char* path);

void create_dir(ErrorCollector errors, const char* name);

#endif  // !GENC_FILE_SYSTEM_H
