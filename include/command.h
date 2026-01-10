#ifndef GENC_COMMAND_H
#define GENC_COMMAND_H

typedef enum
{
    COMMAND_TYPE_BUILD,
    COMMAND_TYPE_INIT,
    COMMAND_TYPE_NEW,
    COMMAND_TYPE_HELP,
    COMMAND_TYPE_UNKNOWN,
} CommandType;

typedef struct
{

} CommandResult;

typedef CommandResult (*CommandFunction)();  // this params could be update cuz we don't know how this will be


#endif  // !GENC_COMMAND_H
