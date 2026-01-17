#include <stdio.h>

#include "../include/common.h"

bool prompt_yes_no(const char* prompt)
{
    char input;
    printf("%s (y/n): ", prompt);
    scanf(" %c", &input);

    return (input == 'y' || input == 'Y');
}
