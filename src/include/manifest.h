#ifndef GENC_MANIFEST_H
#define GENC_MANIFEST_H

/*
 * NOTE: here we will have properties that defines our manifest (genc.toml in
 * root) example:
 *  - project name, version, etc...
 *  - dependencies
 *  - custom commands
 */
typedef struct
{

} Manifest;

char* manifest_get_name();
char* manifest_get_version();

#endif  // !GENC_MANIFEST_H
