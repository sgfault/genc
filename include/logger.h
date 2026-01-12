#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

/**
 * @brief Log levels for controlling output verbosity.
 */
typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

/**
 * @brief Set the minimum log level to output.
 * Messages below this level will be ignored.
 * @param level The minimum LogLevel to display.
 */
void log_set_level(LogLevel level);

/**
 * @brief Set the log output file.
 * By default, logs go to stdout/stderr. Pass NULL to reset to default.
 * @param file Pointer to FILE (e.g., fopen("log.txt", "a")).
 */
void log_set_file(FILE* file);

/**
 * @brief Log a formatted message with a given log level.
 * @param level The LogLevel for the message.
 * @param fmt printf-style format string.
 * @param ... Arguments for the format string.
 */
void log_message(LogLevel level, const char* fmt, ...);

#define LOG_DEBUG(fmt, ...) log_message(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) log_message(LOG_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) log_message(LOG_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) log_message(LOG_ERROR, fmt, ##__VA_ARGS__)

#endif  // LOGGER_H
