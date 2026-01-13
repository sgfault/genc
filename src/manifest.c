#include "../include/manifest.h"
#include "../include/logger.h"
#include "../include/toml.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * @brief Helper function to safely duplicate strings
 */
static char* manifest_strdup(const char* str)
{
    if (!str)
        return NULL;
    char* dup = malloc(strlen(str) + 1);
    if (dup)
        strcpy(dup, str);
    return dup;
}

/**
 * @brief Parse the [package] section
 */
static Package* manifest_parse_package(toml_table_t* config)
{
    toml_table_t* pkg_table = toml_table_in(config, "package");
    if (!pkg_table)
        return NULL;

    Package* pkg = calloc(1, sizeof(Package));
    if (!pkg)
        return NULL;

    /* Extract string values from the package table */
    toml_datum_t name = toml_string_in(pkg_table, "name");
    toml_datum_t version = toml_string_in(pkg_table, "version");
    toml_datum_t edition = toml_string_in(pkg_table, "edition");
    toml_datum_t compiler = toml_string_in(pkg_table, "compiler");

    if (name.ok)
        pkg->name = manifest_strdup(name.u.s);
    if (version.ok)
        pkg->version = manifest_strdup(version.u.s);
    if (edition.ok)
        pkg->edition = manifest_strdup(edition.u.s);
    if (compiler.ok)
        pkg->compiler = manifest_strdup(compiler.u.s);

    /* Free temporary strings allocated by toml_string_in */
    if (name.ok)
        free(name.u.s);
    if (version.ok)
        free(version.u.s);
    if (edition.ok)
        free(edition.u.s);
    if (compiler.ok)
        free(compiler.u.s);

    return pkg;
}

/**
 * @brief Parse the [build] section
 */
static Build* manifest_parse_build(toml_table_t* config)
{
    toml_table_t* build_table = toml_table_in(config, "build");

    Build* build = calloc(1, sizeof(Build));
    if (!build)
        return NULL;

    if (build_table)
    {
        toml_datum_t src_dir = toml_string_in(build_table, "src_dir");
        toml_datum_t include_dir = toml_string_in(build_table, "include_dir");
        toml_datum_t bin_dir = toml_string_in(build_table, "bin_dir");

        if (src_dir.ok)
        {
            build->src_dir = manifest_strdup(src_dir.u.s);
            free(src_dir.u.s);
        }
        if (include_dir.ok)
        {
            build->include_dir = manifest_strdup(include_dir.u.s);
            free(include_dir.u.s);
        }
        if (bin_dir.ok)
        {
            build->bin_dir = manifest_strdup(bin_dir.u.s);
            free(bin_dir.u.s);
        }
    }

    // Set defaults for missing fields
    if (!build->src_dir)
        build->src_dir = manifest_strdup("src");
    if (!build->include_dir)
        build->include_dir = manifest_strdup("include");
    if (!build->bin_dir)
        build->bin_dir = manifest_strdup("bin");

    return build;
}

/**
 * @brief Parse the [dependencies] section
 * This is the key function that uses tomlc99's iteration capabilities
 */
static int manifest_parse_dependencies(toml_table_t* config, Dependency** deps_out)
{
    toml_table_t* deps_table = toml_table_in(config, "dependencies");
    if (!deps_table)
    {
        *deps_out = NULL;
        return 0;
    }

    /* Count dependencies by iterating through keys */
    int         count = 0;
    const char* key;
    while ((key = toml_key_in(deps_table, count)) != NULL)
    {
        count++;
    }

    if (count == 0)
    {
        *deps_out = NULL;
        return 0;
    }

    /* Allocate dependency array */
    Dependency* deps = calloc(count, sizeof(Dependency));
    if (!deps)
        return -1;

    // Parse each dependency
    for (int i = 0; i < count; i++)
    {
        key = toml_key_in(deps_table, i);
        if (!key)
            break;

        deps[i].key = manifest_strdup(key);

        // Check if it's a simple string or a table
        toml_datum_t  dep_string = toml_string_in(deps_table, key);
        toml_table_t* dep_table = toml_table_in(deps_table, key);

        if (dep_string.ok)
        {
            // Simple format: genc = "1.0.0"
            deps[i].version = manifest_strdup(dep_string.u.s);
            deps[i].url = NULL;
            deps[i].is_git = false;
            free(dep_string.u.s);
        }
        else if (dep_table)
        {
            // Detailed format: genc = { url = "...", version = "..." }
            toml_datum_t url = toml_string_in(dep_table, "url");
            toml_datum_t version = toml_string_in(dep_table, "version");

            if (url.ok)
            {
                deps[i].url = manifest_strdup(url.u.s);

                // Detect if it's a git dependency
                const char* url_str = url.u.s;
                deps[i].is_git = (strstr(url_str, ".git") != NULL ||
                                  strstr(url_str, "github.com") != NULL ||
                                  strstr(url_str, "gitlab.com") != NULL);
                free(url.u.s);
            }
            else
            {
                deps[i].url = NULL;
                deps[i].is_git = false;
            }

            if (version.ok)
            {
                deps[i].version = manifest_strdup(version.u.s);
                free(version.u.s);
            }
            else
            {
                deps[i].version = NULL;
            }
        }
    }

    *deps_out = deps;
    return count;
}

/**
 * @brief Load and parse a manifest from a TOML file
 */
Manifest* manifest_load(const char* path)
{
    if (!path)
    {
        LOG_ERROR("Error: NULL path provided\n");
        return NULL;
    }

    // Open file
    FILE* fp = fopen(path, "r");
    if (!fp)
    {
        LOG_ERROR("Error: Cannot open manifest file '%s': %s\n", path);
        return NULL;
    }

    // Parse TOML file
    char          errbuf[200];
    toml_table_t* config = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!config)
    {
        LOG_ERROR("Error parsing manifest file '%s': %s\n", path, errbuf);
        return NULL;
    }

    // Allocate manifest structure
    Manifest* manifest = calloc(1, sizeof(Manifest));
    if (!manifest)
    {
        LOG_ERROR("Error: Failed to allocate memory for manifest\n");
        toml_free(config);
        return NULL;
    }

    // Parse each section
    manifest->package = manifest_parse_package(config);
    manifest->build = manifest_parse_build(config);
    manifest->deps_count = manifest_parse_dependencies(config, &manifest->dependencies);
    manifest->custom_attrs_count = 0;
    manifest->custom_attributes = NULL;

    // Validate required fields
    if (!manifest->package || !manifest->package->name)
    {
        LOG_ERROR("Error: Manifest must contain [package] section with 'name' field\n");
        manifest_free(manifest);
        toml_free(config);
        return NULL;
    }

    // Free the TOML configiguration
    toml_free(config);

    return manifest;
}

/**
 * @brief Resolve dependencies (download, validate, etc.)
 */
void manifest_resolve_dependencies(Manifest* manifest)
{
    if (!manifest || manifest->deps_count <= 0)
    {
        LOG_INFO("No dependencies to resolve.\n");
        return;
    }

    LOG_INFO("Resolving %d dependencies...\n", manifest->deps_count);

    for (int i = 0; i < manifest->deps_count; i++)
    {
        Dependency* dep = &manifest->dependencies[i];

        printf("  [%d/%d] %s", i + 1, manifest->deps_count, dep->key);

        if (dep->version)
        {
            printf(" (version: %s)", dep->version);
        }

        if (dep->url)
        {
            printf(" from %s", dep->url);
        }

        printf("\n");

        /*  TODO: Implement actual resolution logic:
         1. Check local cache: ~/.buildtool/cache/<package-name>/<version>
         2. For git dependencies:
            - Clone if not exists: git clone <url> cache/<name>
            - Pull updates: git fetch && git checkout <version>
         3. For version dependencies:
            - Download from registry/repository
            - Verify version satisfies semver requirement
         4. Build dependency graph for:
            - Cycle detection
            - Topological sort for build order
         5. Recursively resolve sub-dependencies
        */

        if (dep->is_git && dep->url)
        {
            printf("      -> Git dependency detected\n");
            // Example implementation:
            // char cmd[512];
            // snprintf(cmd, sizeof(cmd), "git clone %s deps/%s", dep->url, dep->key);
            // system(cmd);
        }
        else if (dep->version)
        {
            printf("      -> Version requirement: %s\n", dep->version);
            // Example: download from package registry
            // download_from_registry(dep->key, dep->version);
        }
    }

    LOG_INFO("Dependency resolution complete.\n");
}

/**
 * @brief Free all memory associated with a manifest
 */
void manifest_free(Manifest* manifest)
{
    if (!manifest)
        return;

    // Free package
    if (manifest->package)
    {
        free(manifest->package->name);
        free(manifest->package->version);
        free(manifest->package->edition);
        free(manifest->package->compiler);
        free(manifest->package);
    }

    // Free build
    if (manifest->build)
    {
        free(manifest->build->src_dir);
        free(manifest->build->include_dir);
        free(manifest->build->bin_dir);
        free(manifest->build);
    }

    // Free dependencies
    if (manifest->dependencies)
    {
        for (int i = 0; i < manifest->deps_count; i++)
        {
            free(manifest->dependencies[i].key);
            free(manifest->dependencies[i].url);
            free(manifest->dependencies[i].version);
        }
        free(manifest->dependencies);
    }

    // Free custom attributes
    if (manifest->custom_attributes)
    {
        for (int i = 0; i < manifest->custom_attrs_count; i++)
        {
            free(manifest->custom_attributes[i].key);
            free(manifest->custom_attributes[i].value);
        }
        free(manifest->custom_attributes);
    }

    free(manifest);
}

/**
 * @brief Print manifest contents for debugging
 */
void manifest_print(const Manifest* manifest)
{
    if (!manifest)
    {
        printf("NULL manifest\n");
        return;
    }

    printf("=== Manifest ===\n");

    if (manifest->package)
    {
        printf("[package]\n");
        if (manifest->package->name)
            printf("  name = \"%s\"\n", manifest->package->name);
        if (manifest->package->version)
            printf("  version = \"%s\"\n", manifest->package->version);
        if (manifest->package->edition)
            printf("  edition = \"%s\"\n", manifest->package->edition);
        if (manifest->package->compiler)
            printf("  compiler = \"%s\"\n", manifest->package->compiler);
    }

    if (manifest->build)
    {
        printf("\n[build]\n");
        if (manifest->build->src_dir)
            printf("  src_dir = \"%s\"\n", manifest->build->src_dir);
        if (manifest->build->include_dir)
            printf("  include_dir = \"%s\"\n", manifest->build->include_dir);
        if (manifest->build->bin_dir)
            printf("  bin_dir = \"%s\"\n", manifest->build->bin_dir);
    }

    if (manifest->deps_count > 0)
    {
        printf("\n[dependencies] (%d total)\n", manifest->deps_count);
        for (int i = 0; i < manifest->deps_count; i++)
        {
            Dependency* dep = &manifest->dependencies[i];
            printf("  %s", dep->key);
            if (dep->version && dep->url)
            {
                printf(" = { version = \"%s\", url = \"%s\" }", dep->version, dep->url);
            }
            else if (dep->version)
            {
                printf(" = \"%s\"", dep->version);
            }
            if (dep->is_git)
                printf(" [git]");
            printf("\n");
        }
    }

    printf("================\n");
}
