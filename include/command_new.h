#ifndef GENC_COMMAND_NEW_H
#define GENC_COMMAND_NEW_H

#include "command.h"
#include "common.h"
#include "errors.h"

inline CommandResult command_new()
{
    CommandResult result;
    return result;
}

/**
 * Initialize a project
 * @param collector: error collector to add errors to if any
 * @param type: type of error
 * @param _template: template to use to initialize the project
 * @param attribute: the value to use to replace the placeholder (could be name, author or any other attribute)
 * @param placeholder: the placeholder used in the template eg: {{project_name}}
 */
void create_new_project(ErrorCollector* collector, const char* name);

#endif  // !GENC_COMMAND_NEW_H
