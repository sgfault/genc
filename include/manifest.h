#ifndef GENC_MANIFEST_H
#define GENC_MANIFEST_H

#include "stb_ds.h"
#include "toml.h"
#include <stdbool.h>

/** Improved struct definitions for Manifest components **/
typedef struct
{
    char* key;
    char* url;
    char* version;
    bool  is_git;
} Dependency;

/** Struct representing a key-value pair for custom manifest attributes **/
typedef struct
{
    char* key;
    char* value;
} ManifestAttribute;

/** Struct representing package information **/
typedef struct
{
    char* name;
    char* version;
    char* edition;
    char* compiler;
} Package;

/** Struct representing build configuration **/
typedef struct
{
    char* src_dir;
    char* include_dir;
    char* bin_dir;
} Build;

/** Main Manifest struct **/
typedef struct
{
    Package*           package;
    Build*             build;
    Dependency*        dependencies;
    ManifestAttribute* custom_attributes;
    int                deps_count;
    int                custom_attrs_count;
} Manifest;

/**
 * @brief: Safely duplicate a string, returns NULL if input is NULL
 * @param: const char* str: input string
 * @return char*: duplicated string or NULL
 */
static char* manifest_strdup(const char* str);

/**
 * @brief: Get a string value from a TOML table by key
 * @param: toml_table_t* table: pointer to the TOML table
 * @param: const char* key: key to look up
 * @return char*: string value or NULL if not found
 */
static char* manifest_toml_get_string(toml_table_t* table, const char* key);

/**
 * @brief Parse the [package] section
 * @param toml_table_t* root: pointer to the root TOML table
 * @return Package*: pointer to the Package struct
 */
static Package* manifest_parse_package(toml_table_t* root);

/**
 * @brief Parse the [build] section
 * @param toml_table_t* root: pointer to the root TOML table
 * @return Build*: pointer to the Build struct
 */
static Build* manifest_parse_build(toml_table_t* root);

/**
 * @brief Parse the [dependencies] section
 * @param toml_table_t* root: pointer to the root TOML table
 * @param Dependency** deps_out: output pointer for dependencies array
 * @return int: number of dependencies parsed
 */
static int manifest_parse_dependencies(toml_table_t* root, Dependency** deps_out);

/**
 * @brief Parse custom attributes from the manifest
 * @param toml_table_t* root: pointer to the root TOML table
 * @param ManifestAttribute** attrs_out: output pointer for attributes array
 * @return int: number of custom attributes parsed
 */
static int manifest_parse_custom_attributes(toml_table_t* root, ManifestAttribute** attrs_out);

/**
 * @brief Load a manifest from a TOML file
 * @param const char* path: path to the TOML file
 * @return Manifest*: pointer to the loaded Manifest struct
 */
Manifest* manifest_load(const char* path);

/**
 * @brief Resolve dependencies defined in the manifest
 * @param Manifest* manifest: pointer to the Manifest struct
 * @return void
 */
void manifest_resolve_dependencies(Manifest* manifest);

/**
 * @brief Free all memory associated with a manifest
 * @param Manifest* manifest: pointer to the Manifest struct
 */
void manifest_free(Manifest* manifest);

/**
 * @brief Print manifest contents for debugging
 * @param const Manifest* manifest: pointer to the Manifest struct
 */
void manifest_print(const Manifest* manifest);

#endif  // GENC_MANIFEST_H
