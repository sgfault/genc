#include <stdlib.h>

#include "../include/command_init.h"
#include "../include/common.h"
#include "../include/file_system.h"
#include "../include/logger.h"
#include "../include/templates.h"

void init_project(ErrorCollector* err_collector)
{
    if (file_exists("genc.toml"))
    {
        LOG_WARN("Current directory already contains [genc.toml]");
        if (!prompt_yes_no("Do you want to override it?"))
        {
            LOG_INFO("Initialization canceled");
            return;
        }
        LOG_INFO("Overriding existing genc.toml");
    }

    char* dir_name = get_current_dirname();
    if (!dir_name)
    {
        add_error(err_collector, FS_ERR_CANNOT_READ_FILE, "Failed to get current directory name", "");
        report_and_exit(err_collector);
    }

    char* genc_toml = replace_placeholder(genc_toml_template, dir_name, "{{project_name}}");
    write_to_file(err_collector, "genc.toml", genc_toml);

    free(dir_name);
    free(genc_toml);

    LOG_INFO("Initialized genc project in current directory");
}
