#include "../include/command_new.h"
#include "../include/errors.h"
#include "../include/file_system.h"
#include "../include/logger.h"
#include "../include/templates.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * @Author: sidati NOUHI
 * @Date: 12/01/2026
 * Initializes the project with the standard directories src/ include/ and files (formatter, main, readme, gitignore and genc.toml).
 */
void create_new_project(ErrorCollector* errorsCollector, const char* name)
{
    create_dir(errorsCollector, name);
    char path[256];

    snprintf(path, sizeof(path), "%s/src", name);
    create_dir(errorsCollector, path);

    snprintf(path, sizeof(path), "%s/include", name);
    create_dir(errorsCollector, path);

    // main.c
    snprintf(path, sizeof(path), "%s/src/main.c", name);
    char* main_c = replace_placeholder(main_c_template, name, "{{project_name}}");
    write_to_file(errorsCollector, path, main_c);
    free(main_c);

    // README.md
    snprintf(path, sizeof(path), "%s/README.md", name);
    char* readme = replace_placeholder(readme_template, name, "{{project_name}}");
    write_to_file(errorsCollector, path, readme);
    free(readme);

    // .gitignore
    snprintf(path, sizeof(path), "%s/.gitignore", name);
    write_to_file(errorsCollector, path, gitignore_template);

    // genc.toml
    snprintf(path, sizeof(path), "%s/genc.toml", name);
    char* toml = replace_placeholder(genc_toml_template, name, "{{project_name}}");
    write_to_file(errorsCollector, path, toml);
    free(toml);

    // .clang-format
    snprintf(path, sizeof(path), "%s/.clang-format", name);
    char* formatter = replace_placeholder(genc_clang_format_template, name, "{{project_name}}");
    write_to_file(errorsCollector, path, formatter);
    free(formatter);

    LOG_INFO("Project %s created successfully.", name);
}
