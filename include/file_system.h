#ifndef GENC_FILE_SYSTEM_H
#define GENC_FILE_SYSTEM_H

// panic("not implemented yet, go use linux");

void read_file(const char* path);

// just create file if does not exist, if exists ignore it
void create_file(const char* path);

// write to a file, create it if does not exist then write to it
void write_to_file(const char* path, const char* content);

// walk directories recusive...
void walk_dir();

#endif  // !GENC_FILE_SYSTEM_H
