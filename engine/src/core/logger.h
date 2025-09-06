#pragma once

/**
 * @file logger.h
 * @brief Contains the logging system
 *
 * This file provides a simple, configurable logging interface with multiple
 * levels of verbosity. It is designed to be lightweight and allows for
 * compile-time removal of certain log levels to reduce binary size and
 * improve performance in release builds.
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "defines.h"

/** @brief Switch to enable or disable warning-level logs. */
#define LOG_WARN_ENABLED 1

/** @brief Switch to enable or disable info-level logs. */
#define LOG_INFO_ENABLED 1

/** @brief Switch to enable or disable debug-level logs. */
#define LOG_DEBUG_ENABLED 1

/** @brief Switch to enable or disable trace-level logs. */
#define LOG_TRACE_ENABLED 1

// Disable debug and trace logging for release builds.
#if KRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

/**
 * @enum log_level
 * @brief Represents the verbosity level of a log message.
 */
typedef enum log_level {
    /** @brief Fatal error. The application cannot continue and will crash. */
    LOG_LEVEL_FATAL = 0,

    /** @brief A serious error, but the application might recover or continue in a degraded state. */
    LOG_LEVEL_ERROR = 1,

    /** @brief A warning about a sub-optimal condition that does not prevent normal operation. */
    LOG_LEVEL_WARN  = 2,

    /** @brief General informational messages for the developer. */
    LOG_LEVEL_INFO  = 3,

    /** @brief Detailed information for debugging purposes. Only active in debug builds. */
    LOG_LEVEL_DEBUG = 4,
    
    /** @brief Highly verbose, low-level detail for intensive debugging. Only active in debug builds. */
    LOG_LEVEL_TRACE = 5,
} log_level;

/**
 * @brief Initializes the logging system.
 *
 * This function sets up the logging system. In the future, this will handle
 * tasks like creating and opening a log file.
 * @return b8 Returns TRUE if initialization was successful; otherwise, FALSE.
 */
b8 initialize_logging();


/**
 * @brief Shuts down the logging system.
 *
 * Performs any necessary cleanup. In the future, this will handle tasks like
 * writing any queued log entries to disk and closing the log file.
 */
void shutdown_logging();


/**
 * @brief The core function for outputting log messages.
 * @note This function is not intended to be called directly. Use the provided
 * logging macros (e.g., K_FATAL, K_INFO) instead.
 *
 * @param level The log_level of the message.
 * @param message The message to be logged, which can include format specifiers.
 * @param ... Variadic arguments corresponding to the format specifiers in the message.
 */
KAPI void log_output(log_level level, const char* message, ...);


/**
 * @brief Logs a fatal-level message. Application will terminate.
 * @param message The message format string.
 * @param ... Variadic arguments for the format string.
 */
#define KFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);


#ifndef KERROR
/**
 * @brief Logs an error-level message.
 * @param message The message format string.
 * @param ... Variadic arguments for the format string.
 */
#define KERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif


#if LOG_WARN_ENABLED == 1
/**
 * @brief Logs a warning-level message.
 * @param message The message format string.
 * @param ... Variadic arguments for the format string.
 */
#define KWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
/** @brief Compiles to nothing when LOG_WARN_ENABLED is not 1. */
#define KWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
/**
 * @brief Logs an info-level message.
 * @param message The message format string.
 * @param ... Variadic arguments for the format string.
 */
#define KINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
/** @brief Compiles to nothing when LOG_INFO_ENABLED is not 1. */
#define KINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
/**
 * @brief Logs a debug-level message.
 * @param message The message format string.
 * @param ... Variadic arguments for the format string.
 */
#define KDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
/** @brief Compiles to nothing when LOG_DEBUG_ENABLED is not 1. */
#define KDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
/**
 * @brief Logs a trace-level message.
 * @param message The message format string.
 * @param ... Variadic arguments for the format string.
 */
#define KTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
/** @brief Compiles to nothing when LOG_TRACE_ENABLED is not 1. */
#define KTRACE(message, ...)
#endif