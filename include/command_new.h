#ifndef GENC_COMMAND_NEW_H
#define GENC_COMMAND_NEW_H

#include "common.h"
#include "errors.h"

CommandResult command_new();

void init_project(ErrorCollector* errorCollector, const char* name);

char* replace_project_name(const char* _template, const char* project_name);

#endif  // !GENC_COMMAND_NEW_H
