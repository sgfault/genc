#include "../include/logger.h"
#include <stdarg.h>
#include <stdlib.h>

static LogLevel current_level = LOG_DEBUG;
static FILE*    log_file = NULL;

static const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
static const char* level_colors[] = {
    "\033[1;34m",  // DEBUG - Blue
    "\033[1;32m",  // INFO  - Green
    "\033[1;33m",  // WARN  - Yellow
    "\033[1;31m"   // ERROR - Red
};

/**
 * @brief Set the minimum log level to output.
 */
void log_set_level(LogLevel level)
{
    current_level = level;
}

/**
 * @brief Set the log output file.
 * @param file to set as the log file
 */
void log_set_file(FILE* file)
{
    log_file = file;
}

/**
 * @brief Log a formatted message with a given log level.
 */
void log_message(LogLevel level, const char* fmt, ...)
{
    if (level < current_level)
        return;

    FILE* out = log_file ? log_file : (level == LOG_ERROR ? stderr : stdout);

    /* NOTE: These lines are commented for later use (date in log files) cuz for now log files are not used. */
    /*  time_t     now = time(NULL);
     *  struct tm* t = localtime(&now);
     *  char       timebuf[20];
     *  strftime(timebuf, sizeof(timebuf), "%Y-%d-%d %H:%M:%S", t);
     */

    fprintf(
        out,
        "%s [%s]\033[0m ",
        level_colors[level],
        // timebuf,
        level_names[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);

    fprintf(out, "\n");
    fflush(out);
}
