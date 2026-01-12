#ifndef GENC_COMMAND_NEW_H
#define GENC_COMMAND_NEW_H

#include "common.h"
#include "errors.h"

CommandResult command_new();

void init_project(ErrorCollector* errorCollector, const char* name);

char* replace_placeholder(const char* _template, const char* attribute, const char* placeholder);

#endif  // !GENC_COMMAND_NEW_H
