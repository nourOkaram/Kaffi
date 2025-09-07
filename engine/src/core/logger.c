/**
 * @file logger.c
 * @brief Implementation of the logging system.
 * @copyright Copyright (c) 2025
 *
 */

#include "logger.h"
#include "asserts.h"

// TODO: These standard library includes are temporary and will be replaced
// with platform-specific implementations.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/**
 * @brief Initializes the logging system.
 * @note Currently a stub. Future implementations will handle file I/O.
 */
b8 initialize_logging() {
    // TODO: create log file.
    return TRUE;
}


/**
 * @brief Shuts down the logging system.
 * @note Currently a stub. Future implementations will write queued entries to a file.
 */
void shutdown_logging() {
    // TODO: cleanup logging/write queued entries
}



/**
 * @brief The core implementation for outputting log messages.
 *
 * This function formats a message by prepending the appropriate log level string
 * and appending a newline character. It then outputs the result to the console.
 *
 * @note This function uses a large, static buffer on the stack (32k) to avoid
 * the performance overhead of dynamic memory allocation. This imposes a
 * technical limit on the length of a single log entry.
 *
 * @note A specific workaround using `__builtin_va_list` is employed because
 * Microsoft's headers can conflict with the standard `va_list` type expected
 * by GCC/Clang, causing compilation errors.
 *
 * @param level The log_level of the message.
 * @param message The message to be logged, which can include format specifiers.
 * @param ... Variadic arguments corresponding to the format specifiers in the message.
 */
void log_output(log_level level, const char* message, ...) {
    const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    // b8 is_error = level < 2;

    // Buffer for the final formatted message.
    char out_message[32000];
    memset(out_message, 0, sizeof(out_message));

    // If the buffer size is 1, there's no space for any content, 
    if (sizeof(out_message) == 1) {
        printf("%s", out_message);
        return;
    }

    // Prepend the log level string to the buffer.
    size_t prefix_len = snprintf(out_message, sizeof(out_message), "%s", level_strings[level]);
    prefix_len = CLAMP(prefix_len, 0, sizeof(out_message) - 1);

    // Process the variadic arguments and format the original message.
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    size_t msg_len = (size_t)vsnprintf(out_message + prefix_len, sizeof(out_message) - prefix_len, message, arg_ptr);
    msg_len = CLAMP(msg_len, 0, sizeof(out_message) - prefix_len - 1);
    va_end(arg_ptr);
    
    // Ensure the message is properly null-terminated and has a newline.
    size_t total_len = prefix_len + msg_len;
    total_len = CLAMP(total_len, 0, sizeof(out_message) - 2);
    out_message[total_len] = '\n';
    out_message[total_len + 1] = '\0';

    // TODO: This direct printf will be replaced with platform-specific output routines
    // that can handle things like colored console output or writing to different streams.
    printf("%s", out_message);
}



/**
 * @brief Reports an assertion failure by logging a fatal error.
 *
 * This function is called by the assertion macros in asserts.h when an
 * assertion condition is not met. It formats a detailed failure message
 * and logs it using the FATAL log level, which will terminate the application.
 *
 * @param expression The stringified expression that failed.
 * @param message A custom message providing context for the failure.
 * @param file The source file where the assertion occurred.
 * @param line The line number in the source file.
 */
void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
{
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, line: %d\n", expression, message, file, line);
}